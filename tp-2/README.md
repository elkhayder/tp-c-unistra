# C Synth

###### Oscillators
- [x] Sine
- [x] Triangle
- [x] Sawtooth
- [x] Square
- [x] Noise

###### Instruments:
- [x] Multiple Oscillators
- [x] ADSR Envelope
- [x] Biquad filters
- [ ] Parse from .ins files

###### MIDI:
- [x] Parsing
- [ ] Multiple channels
- [ ] Multiple Tracks

###### Export:
- [x] Wav
- [x] CSV
- 
```makefile
# Any line that starts with a hash symbol (#) is considered a comment
# First line consists of Envelope params
AttackDuration AttackAmplitude DecayDuration ReleaseDuration
# Second one contains Filter Params, Put a a dash character (-) to ignore
# There are 5 different filter types
# LowPass = 0, HighPass = 1, BandPass = 2, Notch = 3, LowShelf = 4, HighShelf = 5,
Type CuttingFrequency Q GainInDB
# Oscillators
# Each line consists of Two values, Oscillator type id & Coefficient
# Theoretically there can be unlimited amount of Oscillators, but thecnically no oscillator should be repeated, since the result of a repeated oscillator results in the same oscillator with sum of coefs
# There are 5 different oscillator types
# Sine: 0, Square: 1, Triangle = 2, Sawtooth = 3, Noise = 4
# The coefficients dont have to add up to 1,  they can be more or less since we always normalize the sum of coefs for a single instrument
Type Coef
Type Coef
Type Coef
Type Coef

# Each Instrument shuld be seperated with an empty line, and the cycle repeats for up to 16 Instruments, which it the same maximum of MIDI channels
#
# Example for a single Instrument declaration
0.1 1.1 0.05 0.2
0 1500 0.707 0
0 0.4
1 0.7
2 0.5
3 0.1
4 0.1

0.04 1 0.5 0.1
0 7500 0.707 0
0 1
1 0.7
2 0.5
```