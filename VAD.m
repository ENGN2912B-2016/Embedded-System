function [ output ] = VAD(data, fs)
% INPUTS:   data     - (n x 1) array
%           fs       - sampling frequency of data
% OUTPUT:   output   - (n x 1) binary array, 1s indicate speech
%                       segments in the corresponding input audio file

%10 ms frames
framelength = 0.01*fs;
numframes = ceil(length(data)/framelength);

%pad with zeros if data is not an integer number of frames
if(numframes*framelength > length(data))
    data(end + 1 : numframes*framelength) = 0;
end

%how many frames without speech at the beginning (10s / 10ms frames)
delay = 10/.01;

%indeces for 300Hz lower cutoff and 3600Hz upper cutoff
flo = round(.02*framelength);
fhi = round(.36*framelength);

%instantiate data arrays
output = zeros(length(data),1);
E = NaN(numframes,1);
F = NaN(numframes,1);
sfm = NaN(numframes,1);

%useful to see effectiveness of each metric
Ecount = 0;
Fcount = 0;
sfmcount = 0;

%counts for cumulative no-speech and consecutive frames of speech/no-speech
silence_count = delay;
consec_silence = 0;
consec_speech = 0;

%mean no-speech Energy and SFM
E_min = 0;
sfm_min = 0;

for i = 1:numframes
    %grab a new frame
    frame = data( (i-1)*framelength + 1 : i*framelength );
    
    %take the fft
    f_frame = abs(fft(frame));
    
    %calculate the energy
    E(i) = 20*log10(sum(f_frame(3:35)));
    
    %find the frequency of the maximum component
    [~, Findex] = max(f_frame);
    F(i) = Findex;
    
    %calculate the SFM
    gframe = f_frame(flo:fhi);
    g = geomean(gframe(gframe~=0));
    a = mean(f_frame(flo:fhi));
    sfm(i) = -20*log10(g/a);
    
    %set min values during first frames of no speech
    if(i < delay)
        E_min = (E_min*(i-1) + E(i))/i;
        sfm_min = (sfm_min*(i-1) + sfm(i))/i;
    %After pre-processing, choose threshold values for Energy and SFM
    elseif(i == delay)
        E_thresh = std(E(1:delay));
        sfm_thresh = std(sfm(1:delay));
    else
        %Check features against thresholds
        count = 0;
        if(E(i) - E_min >= E_thresh)
            Ecount = Ecount + 1;
            count = count + 1;
        end
        if(F(i) >= flo && F(i) <= fhi)
            Fcount = Fcount + 1;
            count = count + 1;
        end
        if(sfm(i) - sfm_min >= sfm_thresh)
            sfmcount = sfmcount + 1;
            count = count + 1;
        end
        
        %If 2 metrics met, set as speech
        if(count > 1)
            consec_speech = consec_speech + 1;
            if(consec_speech > 5)
                consec_silence = 0;
                output( (i-1)*framelength + 1 : i*framelength ) = 1;
            end
        else
            consec_silence = consec_silence + 1;
            if(consec_silence < 12)
                output( (i -1)*framelength + 1 : i*framelength ) = 1;
            else
                consec_speech = 0;
                %update silence means
                E_min = (silence_count*E_min + E(i))/(silence_count + 1);
                sfm_min = (silence_count*sfm_min + sfm(i))/(silence_count + 1);
                silence_count = silence_count + 1;
            end
        end
    end
end

% disp(E_min);
% disp(sfm_min);
% Ecount
% Fcount
% sfmcount
% figure
% subplot(2,1,1)
% plot((1:length(data))/framelength,data)
% title('Test03')
% xlabel('Frame Index')
% 
% subplot(2,1,2)
% hold on
% plot(E)
% plot(ones(length(E),1)*(E_thresh + E_min),'-r')
% title('Short-Term Energy')
% xlabel('Frame Index')
% ylabel('dB')

% subplot(2,1,1)
% hold on
% plot(F)
% plot(ones(length(F),1)*flo,'-r')
% plot(ones(length(F),1)*fhi,'-r')
% title('Dominant Frequency')
% xlabel('Frame Index')
% ylabel('Frequency Index')
% 
% subplot(2,1,2)
% hold on
% plot(sfm)
% plot(ones(length(sfm),1)*(sfm_thresh + sfm_min),'-r')
% title('Spectral Flatness')
% xlabel('Frame Index')
% ylabel('-dB')

figure('Position', [315 558 925 420])
t = 0:1/Fs:(length(data)-1)/Fs;
h(1) = plot(t,data);
hold on
h(2) = plot(t,output);
legend(h([1 2]), 'Original Sound', 'VAD Sound', 'Location', 'NorthEastOutside')
xlim([0 max(t)])
xlabel('Time (s)')
ylabel('Sound Pressure')
end