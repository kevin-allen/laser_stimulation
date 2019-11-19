nChannels<-1
nSamples<-40000
samplingRate<-20000
fileName="~/repo/laser_stimulation/src/a.dat"

# combine 2 signals
# signal 1
freq1<-4
amp1<-400
# signal 2
freq2<-20
amp2<-100

signal1<-sin(seq(from=0,to=2*pi*(nSamples/samplingRate)*freq1,length.out = nSamples))*amp1
signal2<-sin(seq(from=0,to=2*pi*(nSamples/samplingRate)*freq2,length.out = nSamples))*amp2
signal<-signal1+signal2-min(signal1+signal2) # dc shift to avoid having negative number
plot(signal1)
plot(signal2)
plot(signal)

f<-file(fileName,"wb")
writeBin(object=as.integer(signal),con=f,size=2)
close(f)
