#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avif/avif.h>
#include <webp/encode.h>

void save_webp(const uint8_t *rgba, int width, int height, const char *output_path) {
    // Compress the image as WebP with lossless compression
    uint8_t *webp_data = NULL;
    size_t webp_size = WebPEncodeLosslessRGBA(rgba, width, height, width * 4, &webp_data);

    if (webp_size == 0) {
        fprintf(stderr, "a2w: failed to encode WebP.\n");
        free(webp_data);
        exit(1);
    }

    // Write the WebP data to the output file
    FILE *file = fopen(output_path, "wb");
    if (!file) {
        perror("a2w: failed to open output file");
        free(webp_data);
        exit(1);
    }

    fwrite(webp_data, webp_size, 1, file);
    fclose(file);
    WebPFree(webp_data);

    printf("a2w: saved WebP image to %s\n", output_path);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input.avif output.webp\n", argv[0]);
        return 1;
    }

    const char *input_path = argv[1];
    const char *output_path = argv[2];

    // Initialize avifDecoder
    avifDecoder *decoder = avifDecoderCreate();
    if (!decoder) {
        fprintf(stderr, "a2w: failed to create AVIF decoder.\n");
        return 1;
    }

    // Read and decode the AVIF file
    avifResult result = avifDecoderSetIOFile(decoder, input_path);
    if (result != AVIF_RESULT_OK) {
        fprintf(stderr, "a2w: failed to set AVIF file: %s\n", avifResultToString(result));
        avifDecoderDestroy(decoder);
        return 1;
    }

    result = avifDecoderParse(decoder);
    if (result != AVIF_RESULT_OK) {
        fprintf(stderr, "a2w: failed to parse AVIF image: %s\n", avifResultToString(result));
        avifDecoderDestroy(decoder);
        return 1;
    }

    printf("a2w: parsed AVIF: %ux%u (%ubpc)\n", decoder->image->width, decoder->image->height, decoder->image->depth);

    result = avifDecoderNextImage(decoder);
    if (result != AVIF_RESULT_OK) {
        fprintf(stderr, "a2w: failed to decode AVIF image: %s\n", avifResultToString(result));
        avifDecoderDestroy(decoder);
        return 1;
    }

    // Get the decoded image data
    avifRGBImage rgb_image;
    avifRGBImageSetDefaults(&rgb_image, decoder->image);

    avifRGBImageAllocatePixels(&rgb_image);
    avifImageYUVToRGB(decoder->image, &rgb_image);

    if (rgb_image.depth > 8) {
        fprintf(stderr, "a2w: bit depth > 8 is not supported!\n");
        avifRGBImageFreePixels(&rgb_image);
        avifDecoderDestroy(decoder);
        return 1;
    } 

    printf("a2w: converting to webp...\n");
    // Convert to WebP and save
    save_webp(rgb_image.pixels, rgb_image.width, rgb_image.height, output_path);


    printf("a2w: done! cleaning up...\n");
    // Cleanup
    avifRGBImageFreePixels(&rgb_image);
    avifDecoderDestroy(decoder);

    return 0;
}

