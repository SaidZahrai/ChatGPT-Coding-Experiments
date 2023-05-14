https://gist.github.com/jambonn/1f5fffc23f97f8413372a438739c1bff

1. sudo apt-get install autoconf automake libtool curl make g++ unzip -y
2. Download the appropriate release here:
https://github.com/protocolbuffers/protobuf/releases/tag/v3.5.1 <protobuf-all-3.5.1.tar.gz
>
3. Unzip the folder
4. Enter the folder and run ./autogen.sh && ./configure && make
5. Then run these other commands. They should run without issues:
$ make check
$ sudo make install
$ which protoc
$ sudo ldconfig
$ protoc --version

https://github.com/grpc/grpc/blob/master/BUILDING.md#install-after-build

https://github.com/grpc/grpc/blob/master/BUILDING.md

