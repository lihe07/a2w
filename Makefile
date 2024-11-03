a2w: main.c ./libavif/build/libavif.a ./libwebp/src/libwebp.a
	gcc -O2 -o ./a2w ./main.c -L./libavif/build/ -L ./libwebp/src/ -lavif -lwebp -static -lpthread -lm

./libavif/build/libavif.a:
	cmake -S libavif -B libavif/build -DBUILD_SHARED_LIBS=OFF -DAVIF_CODEC_AOM=LOCAL -DAVIF_LIBYUV=LOCAL -DAVIF_LIBSHARPYUV=LOCAL -DAVIF_JPEG=LOCAL -DAVIF_ZLIBPNG=LOCAL -DAVIF_BUILD_APPS=ON -DCMAKE_C_FLAGS_RELEASE="-static" -DCMAKE_EXE_LINKER_FLAGS="-static"
	cmake --build libavif/build --parallel

./libwebp/src/libwebp.a:
	cd ./libwebp/ && make -f makefile.unix

test: a2w
	./a2w ./hato.avif ./hato.webp
