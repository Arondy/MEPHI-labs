#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

#include <time.h>

int isJpegFile(const char *filename) {
    const char *extensionJPG = ".jpg";
    const char *extensionJPEG = ".jpeg";
    const char *resultJPG = strstr(filename, extensionJPG);
    const char *resultJPEG = strstr(filename, extensionJPEG);
    return (resultJPG != NULL) || (resultJPEG != NULL);
}

//extern void mirrorImage(int width, int height, int channels, unsigned char *data);

void mirrorImage(int width, int height, int channels, unsigned char *data){
    for (int y = 0; y < height; ++y){               // y - строка
        for (int x = 0; x < width / 2; ++x){        // x - столбец
            for (int c = 0; c < channels; ++c){
                int left_index = (y * width + x) * channels + c;
                int right_index = (y * width + (width - 1 - x)) * channels + c;
                unsigned char temp = data[left_index];
                data[left_index] = data[right_index];
                data[right_index] = temp;
            }
        }
    }
}


int main(int argc, char *argv[]){
    if (argc < 3){
        printf("Not enough args!\n");
        return 1;
    }

    char *input_filename = argv[1];
    char *output_filename = argv[2];

    if (!isJpegFile(input_filename)) {
        printf("Error: Input file must be a JPEG file.\n");
        return 1;
    }

    if (!isJpegFile(output_filename)) {
        printf("Error: Output file must be a JPEG file.\n");
        return 1;
    }
	
	int width, height, channels;
	unsigned char *data = stbi_load(input_filename, &width, &height, &channels, 0);

	if (!data){
		printf("Error loading image\n");
		return 1;
	}

	clock_t t1 = clock();
	mirrorImage(width, height, channels, data);
	clock_t t2 = clock();
	printf("%lf\n", ((double) (t2 - t1)) / CLOCKS_PER_SEC);
	stbi_write_jpg(output_filename, width, height, channels, data, width * channels);
	stbi_image_free(data);

	printf("Image saved to %s\n", output_filename);
    return 0;
}