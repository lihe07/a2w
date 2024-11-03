# a2w - convert avif images to webp

a2w is a simple statically linked C program that converts avif images to webp.

It uses `libwebp` and `libavif` for encoding and decoding.

Currently only supports 8-bit depth images.

## Build

- Clone this repo

- Sync submodules: `git submodule sync`

- `make a2w`

## Usage

`a2w ./input.avif ./output.webp`

Can be integrated with other projects. Refer to `main.go`


