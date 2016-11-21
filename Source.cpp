#include <iostream>
#include <fftw3.h> //fft
#include <math.h>
#include <vector>
#include <numeric> //accumulate
#include <algorithm> //max_element, fill
#include <fstream>
#include <cassert>
#include <functional>



//CREATE HAMMING WINDOW CLASS HERE

using namespace std;
#define Fs 44100
#define frameLength 441 //.01*Fs = 10 ms frames
#define nyq 221 //nyquist for fft

double STD(vector<double> data)
{
	double sum = 0;
	double mean = accumulate(data.begin(), data.end(), 0.0)/data.size();
	for (int k = 0; k < data.size(); k++)
	{
		sum = sum + pow(data[k] - mean, 2.0);
	}
	sum = sqrt(sum / (data.size()-1));
	return sum;
}



void load_vec(const string &filename, vector<double> &vec) 
{

	ifstream infile(filename);
	assert(infile.is_open());
	double value;              
	while (infile >> value) {
		vec.push_back(value);
	}
	infile.close();
}

void save_vec(const string &filename, vector<double> &vec)
{

	ofstream outfile(filename);
	assert(outfile.is_open());
	for (vector<double>::const_iterator i = vec.begin(); i != vec.end(); ++i) {
		outfile << *i << '\n';
	}
	outfile.close();
}


int main()
{
	vector<double> input;
	string filename = "testSpeech.txt";
	load_vec(filename, input);


	//10 ms frames
	double numFrames = ceil(input.size() / static_cast<double>(frameLength));

	//pad with zeros if data is not an integer number of frames
	if (numFrames*frameLength > input.size())
	{
		vector<double> zeroPad((numFrames*frameLength) - input.size(), 0);
		input.insert(input.end(), zeroPad.begin(), zeroPad.end());
	}

	//num of frames of silence, assume 2 seconds
	int delay = 2 / .01;

	//indeces for 200Hz lower cutoff and 3600Hz upper cutoff
	int flo = round(.02*frameLength);
	int fhi = round(.36*frameLength);

	//instantiate data arrays
	vector<double> output(input.size());
	vector<double> E(numFrames);
	vector<double> F(numFrames);
	vector<double> sfm(numFrames);

	//counts for cumulative no - speech and consecutive frames of speech / no - speech
	int silence_count = delay;
	int consec_silence = 0;
	int consec_speech = 0;

	//mean no - speech Energy and SFM
	double E_min = 0;
	double sfm_min = 0;
	double E_thresh;
	double sfm_thresh;
	int count = 0; //for number of features pass thresh


	//initialize frame 
	double f_frame_abs[nyq];

	for (int k = 0; k < numFrames; k++)
	{
		//grab a new frame
		vector<double> frame(&input[k*frameLength], &input[(k+1)*frameLength]);

		//APPLY HAMMING WINDOW TO FRAME HERE BEFORE CONTINUING 

		//take the fft
		fftw_complex f_frame[frameLength];
		fftw_plan p = fftw_plan_dft_r2c_1d(frameLength, &frame[0], f_frame, FFTW_ESTIMATE);
		fftw_execute(p);

		for (int j = 0; j < nyq; j++)
		{
			f_frame_abs[j] = sqrt(pow(f_frame[j][0], 2) + pow(f_frame[j][1], 2));
		}

		//calculate the energy
		E[k] = 20 * log10(accumulate(f_frame_abs+2, f_frame_abs+35, 0.0));

		//find the frequency of the maximum component
		F[k] = max_element(begin(f_frame_abs), end(f_frame_abs)) - f_frame_abs;

		//calculate the SFM
		double gMean = 1;
		double aMean = 0;
		int countMult = 0;
		int countAdd = 0;
		for (int j = flo-1; j < fhi; j++)
		{
			aMean = aMean + f_frame_abs[j];
			if (f_frame_abs[j] != 0.0)
			{
				gMean = gMean*f_frame_abs[j];
				countMult = countMult + 1;
			}
			countAdd = countAdd + 1;
		}
		gMean = pow(gMean, 1.0/countMult);
		aMean = aMean / countAdd;
		sfm[k] = -20.0 * log10(gMean / aMean);

		//set min values during first frames of no speech
		if (k < delay)
		{
			E_min = (E_min*k + E[k]) / (k+1);
			sfm_min = (sfm_min*k + sfm[k]) / (k+1);
		}
		else if (k == delay)
		{
			vector<double> Ev(E.begin(), E.begin()+delay-1);
			vector<double> sfmv(sfm.begin(), sfm.begin() + delay - 1);
			E_thresh = STD(Ev);
			sfm_thresh = STD(sfmv);
		}
		else
		{
			if (E[k] - E_min >= E_thresh)
			{
				count = count + 1;
			}
			if ((F[k] >= flo-1 && F[k] <= fhi-1))
			{
				count = count + 1;
			}
			if (sfm[k] - sfm_min >= sfm_thresh)
			{
				count = count + 1;
			}

			if (count > 1) 
			{
				consec_speech = consec_speech + 1;
				if (consec_speech > 5)
				{
					consec_silence = 0;
					fill(output.begin() + k*frameLength, output.begin() + (k + 1)*frameLength - 1, 1);
				}
			}
			else
			{
				consec_silence = consec_silence + 1;
				if (consec_silence < 12)
				{
					fill(output.begin() + k*frameLength, output.begin() + (k + 1)*frameLength - 1, 1);
				}
				else
				{
					consec_speech = 0;
					E_min = (silence_count*E_min + E[k]) / (silence_count + 1);
					sfm_min = (silence_count*sfm_min + sfm[k]) / (silence_count + 1);
					silence_count = silence_count + 1;
				}
			}
		}
	}
	filename = "output.txt";
	save_vec(filename, output);

	return 0;
}



//FFT EXAMPLE USE
//fftw_complex out[N];

//double in[N];
//fftw_plan p;
//for (int i = 0; i < N; i++) {
//	in[i] = cos(3 * 2 * PI*i / N);
//}

//p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);
//fftw_execute(p);