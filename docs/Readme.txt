Contents
    1. What is EAQUAL?
    2. How to use EAQUAL
    3. Description of Model Output Variables (MOVs)
    4. Description of Error Messages
    5. The algorithm
    6. License of EAQUAL
    7. HTML documentation of source code
    
    
    
1. What is EAQUAL?
    The intention of EAQUAL is to provide a objective quality measure for coded/ decoded audio 
    files. Currently the code is based on the ITU-R recommendation BS.1387. 
    EAQUAL will not supersede listening tests, but can be a useful tool to support listening tests, 
    categorize different coding algorithms and find bugs in algorithms. The more input files are 
    taken for the analysis, the better the results of EAQUAL will fit the reality.

2. How to use EAQUAL
    To use EAQUAL, you have to provide the reference file, which is the original PCM data 
    (prefereably 16bit, 48kHz), and the test file, which is the coded and decoded audio file and 
    has the *same* audio format. Invalid file formats are samplerates < 44100Hz or more than two
    channels.
    The most interesting output value of EAQUAL is the ODG (Objective Difference Grade). An ODG 
    of -4 means a very annoying disturbance, while an ODG of 0 means that there is no perceptible 
    difference.
    
    Usage:
        EAQUAL [-options] -fref reference_file_path -ftest test_file_path 
        
    Options:
        -bits   specify number of bits in audio file (only needed with raw input, default: -bits 16)
        -blockout get intermediate results for blocks: -blockout value file_path 
                (valid values are: All, ODG, DI, BWRef, BWTest, NMR, ADB, EHS, ModDiff1, ModDiff2, 
                NoiseLoud, DetProb, RDF, NMRSpec, ModDiffSpec, DetProbSpec, NLSpec). The values will
                be plotted in the text file file_path for debugging purposes
        -ch     specify number of channels in audio file (only needed with raw input, default: -ch 2)
        -forget enable forget factor (only applied for output value MFPD: rate distortions at 
                beginning of audio files as less important)
        -fref   reference (original) audio file
        -ftest  audio file under test
        -h      print help
        -offset shift test buffer for offset samples (that means, discard those samples). If offset <0
                the reference buffer is shifted (default: -offset 0)
        -silent no output to stdio, but in an ASCII file specified after this switch
        -srate  specify sample rate of audio file (only needed with raw input, default: -srate 48000)
        -spl    specify listening level in dBSPL (default: -spl 92)
        -ver    output program version

    Useful tips:
    -   Both input files have to be time aligned. Many decoders add a frame of silence to the 
        beginning of the decoded file. If this is the case, EAQUAL will not work correctly. 
        Please use the ?offset switch to adjust the delay between reference and test file ? if 
        offset is positive then  the test file is delayed and vice versa.
    -   EAQUAL will not work as intended if the file length (of both, reference and test 
        signal) is not sufficient. To achieve reasonable results, a minimal file length of 10-20s 
        is strongly recommended, the longer the better.
    -   The recommendation ITU-R BS.1387 on which EAQUAL is based was developed for 
        high quality codecs with little difference between reference and test signal. If you are 
        trying to use EAQUAL to rank e.g. speech codecs it will fail.  
        If you must use EAQUAL for middle quality codecs then use the DI-output value as 
        ranking, not the ODG (Objective Difference Grade), which is only a shifted and 
        nonlinear (sigmoid) transformed version of the DI. 

3. Description of Model Output Variables (MOVs)
    ODG:        Objective Difference Grade, a measure of quality comparable to the Subjective 
                Difference Grade (SDG), which is calculated as the difference between the quality 
                rating of the reference and the test signal. The quality ratings are measured with the 
                five point scale defined in ITU-R BS.1116 and thus the SDG and ODG have a range 
                of [-4;0] where -4 stands for very annoying difference and 0 stands for imperceptible 
                difference between reference and test signal.
    DI:         Distortion Index, a quality measure based on the calculated Model Output 
                Variables(MOVs). It is calculated with a trained neural network.
    Bandwidth:  estimate of the bandwidths of both, the reference and test signal
    NMR:        Noise-To-Mask-Ratio, ratio between the noise (introduced by the codec) and the 
                allowed masking threshold
    RDF:        Relative Disturbed Frames, based on NMR. If the NMR of any frequency band is 
                higher 1.5dB the frame is assumed to be disturbed. The number of disturbed frames is 
                then divided by the number of all frames
    MFPD:       Maximum Filtered Probability of Detection. A detection probability of hearing the 
                noise/artefacts is calculated similar to the NMR. This probability is smoothed over 
                time by a low pass filter of first order. After that the maximum of the resulting value 
                and the MFPD of the preceding frame is chosen.
    ADB:        Average Distorted Block. Similar to RDF, the number of distorted blocks is calculated 
                based on the detection probability (if detection probability > 0.5). Then a measure of 
                how hearable the distortion is is divided by this number.
    EHS:        Harmonic Structure of Error, a measure how tonal the noise signal is. The calculation is 
                based on the autocorrelation of the error spectrum.
    Modulation Difference: measurement of differences between the envelopes of reference and 
                test signal based on a simple loudness measure and low pass filtering.
    Noise Loudness: a measure of the loudness of the noise signal.
    
    If you understand german, it is recommended to read a paper about ITU-R BS.1398, available at
    http://www-ft.ee.tu-berlin.de/Publikationen/papers/Tonmeistertagung1998.pdf.

4. Description of Error Messages
    
    1000001     memory allocation failed
    
    2000001     division by zero, please contact lerch@zplane.de with the test files
    
    3000001     file open error
    3000002     file close error
    3000003     file read error
    3000004     file write error
    3000006     unknown file format
    3000007     illegal file format
    
    7000001     missing command line argument
    7000002     wrong command line argument

    9999999     unknown error

5. The algorithm 
    EAQUAL uses a psychoacoustic model to calculate the signals at the basilar membran, the so called
    "excitation patterns". The calculation is listening level dependent and uses nonlinear addition of
    masking patterns. Both input files are processed in the model.
    The cognitive model is a combination of different algorithms/models to rate the perceptability of 
    noise and distortion added by the coding algorithm. Each of these algorithms yields to a MOV (model
    output variable); all of them are described above. The average of these MOVs over time and channels
    are the inputs of a neural network, the output of the network is the DI (distortion index). This
    value is mapped through a nonlinear (sigmoid) function to the ODG, the final result.
    

6. License of EAQUAL
    The source was originally developed at Heinrich-Hertz-Institut fuer Nachrichtentechnik and 
    was then published under the GPL. For more information on the GPL read the license 
    included with the source code package.
    Please note that the use of this software may require the payment of
    patent royalties. You need to consider this issue before you start
    building derivative works. We are not warranting or indemnifying you in
    any way for patent royalities! YOU ARE SOLELY RESPONSIBLE FOR YOUR OWN
    ACTIONS!
 
7. HTML documentation of source code
    An automatically generated documentation of classes, functions, etc. is available at 
    /EAQUAL/docs/htm/index.htm
