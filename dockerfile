FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

WORKDIR /home/ubuntu

# Install necessary packages
RUN apt-get update && apt-get install -y \
	wget \
	xz-utils \
	make \
	cppcheck \
	git

# Install arm gnu toolchain
RUN mkdir -p dev/tools tmp \
	&& wget -O tmp/arm-gnu-toolchain.tar.xz https://developer.arm.com/-/media/Files/downloads/gnu/15.2.rel1/binrel/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi.tar.xz \
	&& tar -xf tmp/arm-gnu-toolchain.tar.xz -C dev/tools \
	&& rm tmp/arm-gnu-toolchain.tar.xz

# Add toolchain to PATH
ENV PATH="${PATH}:/home/ubuntu/dev/tools/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi/bin"