#!/usr/bin/env python3

"""
Used to translate mono wav audio binaries into AVR memory data.
Based on the C program by InoSchlaucher wave2c
"""

import sys
import struct

COLUMNS = 12


def load_wav_header(f):
    """
    Unpack the header of a WAVE file and perform basic checks on the data.
    """
    header_fmt = '@4sI4s4sIHHIIHH4sI'
    header_keys = [
        'RIFF',
        'chunkSize',
        'WAVE',
        'fmt ',
        'subChunk1Size',
        'audFormat',
        'nbChannels',
        'sampleRate',
        'byteRate',
        'blockAlign',
        'bitsPerSample',
        'data',
        'subChunk2Size',
    ]
    header = f.read(struct.calcsize(header_fmt))
    header = struct.unpack(header_fmt, header)
    header = dict(zip(header_keys, header))

    if header['RIFF'] != b'RIFF':
        print("Not a RIFF: {}".format(header['RIFF']))
        return None

    if header['WAVE'] != b'WAVE':
        print("Not a WAVE: {}".format(header['WAVE']))
        return None

    if header['fmt '] != b'fmt ':
        print("Not a fmt subchunk: {}".format(header['fmt ']))
        return None

    if header['subChunk1Size'] != 16:
        print("Not PCM chunk size: 0x{:04x}".format(header['subChunk1Size']))
        return None

    if header['audFormat'] != 1:
        print("Not PCM format (1): 0x{:04x}".format(header['audFormat']))
        return None

    if header['nbChannels'] != 1:
        print("Num of channels invalid: 0x{:04x}".format(header['nbChannels']))
        return None

    if header['bitsPerSample'] != 8:
        print("Expected 8bit format: 0x{:04x}".format(header['bitsPerSample']))
        return None

    if header['data'] != b'data':
        print("not a data subchunk 0x{:04x}".format(header['data']))
        return None

    return {
        'sampleRate': header['sampleRate'],
        'numChannels': header['nbChannels'],
        'bitsPerSample': header['bitsPerSample'],
        'dataLength': header['subChunk2Size'],
    }


def save_wave(fin, header, fout, sound_name, samples):
    """
    Load the actual wave data and write it out to the arduino file
    """
    print(header)

    # The real length is the number of bytes used per channel.
    # The original code had the multiplication by 8 at the end which meant that
    # there could be a rounding error which results in length being trimmed to
    # a multiple of 8. This has been removed.
    realLength = int(
        8 *
        header['dataLength'] /
        header['numChannels'] /
        header['bitsPerSample']
    )

    # If a maximum number of samples has been specified then trim to this
    # length.
    if samples != -1 and realLength > samples:
        realLength = samples

    # Basic header information
    fout.write('// {} sound made by wav2c.py\n\n'.format(sound_name))
    fout.write('#include "pcm_audio.h"\n\n')
    fout.write('// sample rate = {}\n'.format(header['sampleRate']))
    fout.write('// length = {}\n\n'.format(realLength))

    # The actual wav data
    fout.write('const uint8_t {}_data[] PROGMEM = {{'.format(sound_name))
    for i in range(realLength):
        sample = struct.unpack('B', fin.read(struct.calcsize('B')))[0]

        # Insert spacing between the bytes. Every COLUMNS bytes uses a newline.
        if i % COLUMNS == 0:
            fout.write('\n    ')
        else:
            fout.write(' ')

        # Hex of fixed width makes it easy to read.
        fout.write('0x{:02x},'.format(sample))

    fout.write('\n};\n\n')

    # Load it into a pcm_audio struct
    fout.write('struct pcm_audio {} = {{\n'.format(sound_name))
    fout.write('    {},\n'.format(header['sampleRate']))
    fout.write('    {},\n'.format(realLength))
    fout.write('    {}_data\n'.format(sound_name))
    fout.write('};\n')


def main(argv=None):
    if argv is None:
        argv = sys.argv

    if len(argv) < 4 or len(argv) > 6:
        print("Usage: {} <input.wav> <output.c> <name> [<samples>]".format(
            argv[0],
        ))

    in_filename = argv[1]
    out_filename = argv[2]
    sound_name = argv[3]
    if len(argv) > 4:
        samples = argv[4]
    else:
        samples = -1

    with open(in_filename, 'rb') as fin:
        header = load_wav_header(fin)
        if header is None:
            quit(1)
        with open(out_filename, 'w') as fout:
            save_wave(fin, header, fout, sound_name, samples)


if __name__ == '__main__':
    main(sys.argv)
