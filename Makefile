build:
	rand=$$RANDOM && \
	xcodebuild && \
	mv ./build/Release/redapple ./$$rand
