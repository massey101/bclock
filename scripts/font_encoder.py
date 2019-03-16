#!/usr/bin/env python3

import math
import sys
import os
from PIL import Image, ImageFont, ImageDraw


def convert_glyffs(font, glyffs, w, h):

    canvas = Image.new('1', (w*len(glyffs), h), 'white')

    draw = ImageDraw.Draw(canvas)
    draw.text((0, 0), ''.join(glyffs), 'black', font)
    px = canvas.load()
    output = {}
    x_offset = 0
    for glyff in glyffs:
        rows = []
        for y in range(h):
            columns = []
            for x in range(w):
                columns.append(px[x + x_offset, y])
            rows.append(columns)
        output[glyff] = rows
        x_offset += w

    return output


def to_hash_format(px, w, h):
    output = []
    for y in range(h):
        row = []
        for x in range(w):
            if px[y][x]:
                row.append(' ')
            else:
                row.append('#')
        output.append(row)

    return output


def to_hex_format(px, w, h):
    hex_width = math.ceil(w / 8)
    output = []
    for y in range(h):
        row = [0] * hex_width
        for x in range(w):
            if not px[y][x]:
                row[math.floor(x / 8)] |= 1 << (7 - (x % 8))
        output.append(row)

    return output


def to_header_format(hashed, hexed):
    output = ''
    for hash_row, hex_row in zip(hashed, hexed):
        output += '    '
        for column in hex_row:
            output += '0x{:02x}, '.format(column)
        output += '// '
        for column in hash_row:
            output += column
        output += '\n'

    return output


def generate_header(font, glyphs, name):
    output = ''
    text = ''.join(glyphs)
    text_width, text_height = font.getsize(text)
    text_width = int(text_width / len(glyphs))
    converted_glyffs = convert_glyffs(font, glyphs, text_width, text_height)

    output += '#include "fonts.h"\n'
    output += '#include <avr/pgmspace.h>\n'
    output += '\n'
    output += 'const uint8_t {}_Table [] PROGMEM = \n'.format(name)
    output += '{\n'
    byte_counter = 0
    for glyph in glyphs:
        output += '    // @{} \'{}\' ({} pixels wide)\n'.format(
            byte_counter,
            glyph,
            text_width,
        )

        converted = converted_glyffs[glyph]
        hash_format = to_hash_format(converted, text_width, text_height)
        hex_format = to_hex_format(converted, text_width, text_height)
        header_format = to_header_format(hash_format, hex_format)
        byte_counter += len(hex_format) * len(hex_format[0])

        output += header_format
        output += '\n'

    output += '};\n'
    output += '\n'
    output += 'sFONT {} = '.format(name) + '{\n'
    output += '    {}_Table,\n'.format(name)
    output += '    {}, /* Width */\n'.format(text_width)
    output += '    {}, /* Height*/\n'.format(text_height)
    output += '};\n'
    return output


def main():
    ascii_chars = [chr(c) for c in range(ord(' '), ord('~'))]

    filename = sys.argv[1]
    name = os.path.splitext(os.path.basename(filename))[0]
    size = int(sys.argv[2])
    font = ImageFont.truetype(filename, size)

    header = generate_header(font, ascii_chars, name + str(size))
    with open(name + str(size) + '.c', 'w') as f:
        f.write(header)


if __name__ == '__main__':
    main()
