#include <sndfile.hh>
#include <iostream>
#include <stdio.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <fstream>
#include <ctime>
using namespace std;
int main()
{
const int format=SF_FORMAT_WAV | SF_FORMAT_PCM_16;
//	const int format=SF_FORMAT_WAV | SF_FORMAT_FLOAT;
	const int channels=1;
	const int sampleRate=22050;
	const char* outfilename="foo.wav";
SndfileHandle outfile(outfilename, SFM_WRITE, format, channels, sampleRate);
const int size = sampleRate*10;
	float data[size];
    ALCdevice *dev[2];
    ALCcontext *ctx;
    ALuint source, buffers[3];
    //float data[5000];
    ALuint buf;
    ALint val =0;

    dev[0] = alcOpenDevice(NULL);
    ctx = alcCreateContext(dev[0], NULL);
    alcMakeContextCurrent(ctx);

    alGenSources(1, &source);
    alGenBuffers(3, buffers);

    /* Setup some initial silent data to play out of the source */
    alBufferData(buffers[0], AL_FORMAT_MONO16, data, size, sampleRate);
    alBufferData(buffers[1], AL_FORMAT_MONO16, data, size, sampleRate);
    alBufferData(buffers[2], AL_FORMAT_MONO16, data, size, sampleRate);

    alSourceQueueBuffers(source, 3, buffers);

    /* If you don't need 3D spatialization, this should help processing time */
    alDistanceModel(AL_NONE);

    dev[1] = alcCaptureOpenDevice(NULL, sampleRate, AL_FORMAT_MONO16, size/2);

    /* Start playback and capture, and enter the audio loop */
    alSourcePlay(source);
    alcCaptureStart(dev[1]);
   ofstream out("test.csv");
   clock_t start, stop;
   float time;
   start=clock();
   cout<<"Start "<<start;

        /* Check if any queued buffers are finished */
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &val);


        /* Check how much audio data has been captured (note that 'val' is the
        * number of frames, not bytes) */
        alcGetIntegerv(dev[1], ALC_CAPTURE_SAMPLES, 1, &val);

        /* Read the captured audio */
        alcCaptureSamples(dev[1], data, val);
	for(int i=0; i<size; i++)
	out<<i<<" "<<data[i]<<"\n";

/* Pop the oldest finished buffer, fill it with the new capture data,
        then re-queue it to play on the source */
      //  alSourceUnqueueBuffers(source, 1, &buf);
      //  alBufferData(buf, AL_FORMAT_MONO16, data, val*2 /* bytes here, not
       // frames */, 22050);
       // alSourceQueueBuffers(source, 1, &buf);

        /* Make sure the source is still playing */
        //alGetSourcei(source, AL_SOURCE_STATE, &val);

        /*if(val != AL_PLAYING)
        {

            alSourcePlay(source);
        }*/

    outfile.write(&data[0], size);

    out.close();
    cout<<"Koniec "<<stop;
    cout<<"\nCzas nagrywania"<<stop-start<<endl;
    /* Shutdown and cleanup */
    alcCaptureStop(dev[1]);
    alcCaptureCloseDevice(dev[1]);

    alSourceStop(source);
    alDeleteSources(1, &source);
    alDeleteBuffers(3, buffers);

    alcMakeContextCurrent(NULL);
    alcDestroyContext(ctx);
    alcCloseDevice(dev[0]);

    return 0;
}
