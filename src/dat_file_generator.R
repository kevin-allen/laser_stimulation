nChannels<-1
nSamples<- 400000
samplingRate<-20000
fileName="~/repo/laser_stimulation/src/a.dat"

# combine 2 signals
# signal 1
freq1<-4
amp1<-seq(from = 0,to=100,length.out = nSamples)
# signal 2
freq2<-20
amp2<-100

signal1<-sin(seq(from=0,to=2*pi*(nSamples/samplingRate)*freq1,length.out = nSamples))*amp1
signal2<-sin(seq(from=0,to=2*pi*(nSamples/samplingRate)*freq2,length.out = nSamples))*amp2
signal<-signal1+signal2-min(signal1+signal2) # dc shift to avoid having negative number
plot(signal1,type='l')
plot(signal2,type='l')
plot(signal,type='l')

f<-file(fileName,"wb")
writeBin(object=as.integer(signal),con=f,size=2)
close(f)



