// FileToHeader.cpp : Defines the entry point for the application.
// By Haydn V. Harach

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <sstream>
using namespace std;


int main(int argc, char* argv[]) {

	// Make sure we actually have an arg to use.
	if (argc < 2) {
		cout << "Not enough arguments.\n";
		return 1;
	}

	// Go through each file that was given.
	for (int whicharg = 1; whicharg < argc; ++whicharg)
	{

		// Turn argv[i] into a file path.
		filesystem::path mypath(argv[whicharg]);
		if (!filesystem::is_regular_file(mypath)) {
			cout << "Not a regular file.\n";
			return 2;
		}

		// Open the file and get its size.
		ifstream infile(mypath, ios::in | ios::binary);
		if (!infile.is_open()) {
			cout << "Can't open input file.\n";
			return 3;
		}

		// Read the file data.
		size_t size = filesystem::file_size(mypath);
		vector<char> filedata(size);
		if (!infile.read(filedata.data(), size)) {
			cout << "Failed to read input file.\n";
			return 4;
		}

		// Open the output file.
		string outpath = argv[whicharg] + string(".h");
		ofstream outfile(outpath, ios::out);
		if (!outfile.is_open()) {
			cout << "Can't open output file.\n";
			return 5;
		}

		// Get the filename we'll be working with and turn any '.' into '_'.
		string filename = mypath.filename().u8string();
		cout << "Writing file '" << filename << "' to '" << filename << ".h'...   ";
		for (char& c : filename) {
			if (c == '.') { c = '_'; }
		}

		// Write the data to the output file.
		outfile << "#define FILE_" << filename << "_SIZE " << filedata.size() << "\n";
		outfile << "static const unsigned char FILE_" << filename << "_DATA[] = {\n";

		int chars_written_this_line = 0;
		const int MAX_CHARS_PER_LINE = 79;
		for (size_t i = 0; i < filedata.size(); ++i) {

			int chars_needed = 1;
			unsigned char thisdata = (unsigned char)filedata[i];
			if (thisdata >= 10) chars_needed++;
			if (thisdata >= 100) chars_needed++;
			if (i + 1 < filedata.size()) chars_needed++;
			if (chars_written_this_line + chars_needed > MAX_CHARS_PER_LINE) {
				outfile << '\n';
				chars_written_this_line = 0;
			}

			outfile << (int)thisdata;
			if (i + 1 < filedata.size()) { outfile << ","; }
			chars_written_this_line += chars_needed;
		}
		outfile << "\n};";
		cout << "done!\n";
	}

	


	return 0;
}
