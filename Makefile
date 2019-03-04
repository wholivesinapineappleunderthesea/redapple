build:
	xcodebuild
	mv ./build/Release/redapple ./$(shell bash -c 'echo $$RANDOM')
	rm -r build
