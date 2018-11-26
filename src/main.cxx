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
  cerr << "[MPASK] Starting 'mpask_exe'" << endl;

  po::options_description description("Allowed options");
  description.add_options()
    ("help,h", "print this help message")
    ("directory,d", po::value<string>(), "working directory, defaults to current directory")
    ("file,f", po::value<string>(), "input file name")
    ("name,n", po::value<string>(), "do search by name")
    ("oid,o", po::value<string>(), "display specific oid")
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
  cerr << "[MPASK] Working directory: '" << workingDirectory << "'" << endl;

	string searchedName;
  if (variables.count("name")) {
    searchedName = variables["name"].as<string>();
  }
  cerr << "[MPASK] Searched name: '" << searchedName << "'" << endl;

	string oidToDisplay;
  if (variables.count("oid")) {
    oidToDisplay = variables["oid"].as<string>();
  }
  cerr << "[MPASK] OID to display: '" << oidToDisplay << "'" << endl;

	string fileName;
  if (variables.count("file")) {
    fileName = variables["file"].as<string>();
  } else {
    cerr << "mpask_exe: missing file operand" << endl;
    cerr << "Try 'mpask_exe --help' for more information." << endl;
    return 1;
  }
  cerr << "[MPASK] File name: '" << fileName << "'" << endl;

  cerr << "[MPASK] Calling master parser" << endl;
  auto object = MasterParser{}(workingDirectory, fileName);
  cerr << "[MPASK] Calling tree builder" << endl;
  auto root = TreeBuilder{}(object);
  cerr << "[MPASK] Printing tree hierarchy" << endl;
  root->printHierarchy(cout, "-  ");

  string dotFile {workingDirectory + "/"s + fileName + ".dot"s};
  cerr << "[MPASK] Printing dot file to '" << dotFile << "'" << endl;
  ofstream dot;
  dot.open(dotFile);
  root->printDotFile(dot);
  dot.close();
  
  if (!searchedName.empty()) {
    cerr << "[MPASK] Searching the tree for name '" << searchedName << "'" << endl;
    try {
      auto result = root->findOidByName(searchedName);
      auto textOid = root->generateOID(result);
      cerr << "[MPASK] Found OID '" << textOid << "'" << endl;
      if (oidToDisplay.empty()) {
        cerr << "[MPASK] Assuming display request for '" << textOid << "'" << endl;
        oidToDisplay = textOid;
      }
    } 
    catch (const std::exception& e) {
      cerr << "[MPASK] Searching failed with the following message: '" << e.what() << "'" << endl;
    }
  }

  if (!oidToDisplay.empty()) {
    cerr << "[MPASK] Trying to display the node '" << oidToDisplay << "'" << endl;
    try {
      auto result = root->findNodeByOID(oidToDisplay);
      cerr << result->getSource() << endl;
    } 
    catch (const std::exception& e) {
      cerr << "[MPASK] Searching failed with the following message: '" << e.what() << "'" << endl;
    }
  }

  cerr << "[MPASK] Closing executable" << endl;
  return 0;
}
