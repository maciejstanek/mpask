#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>

#include "mpask/MasterParser.hxx"
#include "mpask/TreeBuilder.hxx"

using namespace std;
using namespace mpask;
namespace po = boost::program_options;

int
main(int argc, char *argv[])
{
  cerr << "[DBG] Starting 'mpask_exe'" << endl;

  po::options_description description("Allowed options");
  description.add_options()
    ("help,h", "print this help message")
    ("directory,d", po::value<string>(), "working directory, defaults to current directory")
    ("file,f", po::value<string>(), "input file name")
  ;
  po::variables_map variables;
  po::store(po::parse_command_line(argc, argv, description), variables);
	try {
  	po::notify(variables);
	}
  catch(const po::error& e) {
    cerr << "mpask_exe: " << e.what() << endl;
    cerr << "Try 'mpask_exe --help' for more information." << endl;
		return 1;
	}

  if (variables.count("help")) {
    cerr << "Usage: mpask_exe [OPTION]..." << endl;
    cerr << "Process MIB files." << endl;
    cerr << endl;
    cerr << description;
    return 0;
  }
	
	string workingDirectory {"."};
  if (variables.count("directory")) {
    workingDirectory = variables["directory"].as<string>();
  }
  cerr << "[DBG] Working directory: '" << workingDirectory << "'" << endl;

	string fileName;
  if (variables.count("file")) {
    fileName = variables["file"].as<string>();
  } else {
    cerr << "mpask_exe: missing file operand" << endl;
    cerr << "Try 'mpask_exe --help' for more information." << endl;
    return 1;
  }
  cerr << "[DBG] File name: '" << fileName << "'" << endl;

  cerr << "[DBG] Calling master parser" << endl;
  auto object = MasterParser{}(workingDirectory, fileName);
  cerr << "[DBG] Calling tree builder" << endl;
  auto root = TreeBuilder{}(object);
  cerr << "[DBG] Printing tree hierarchy" << endl;
  root->printHierarchy(cout, "-  ");

  string dotFile {workingDirectory + "/"s + fileName + ".dot"s};
  cerr << "[DBG] Printing dot file to '" << dotFile << "'" << endl;
  ofstream dot;
  dot.open(dotFile);
  root->printDotFile(dot);
  dot.close();

  cerr << "[DBG] Closing executable" << endl;
  return 0;
}
