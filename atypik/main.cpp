#include <getopt.h>

#include <algorithm>
#include <execution>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "bxmlparser.h"
#include "error.h"
#include "machinetypes.h"
#include "modelset.h"
#include "pogparser.h"
#include "solverfactory.h"
#include "timemanager.h"
#include "writer.h"

using namespace belem;
using namespace solver;
using namespace std;
using namespace tinyxml2;
using namespace tools;
using genericparser::Context;
using genericwriter::Writer;

void displayHelp(string prog)
{
  cout << "usage " << prog << " <file.(bxml)|(pog)> folders_to_bxml_files"
       << endl;
  cout << "Options:" << endl;
  cout << "--bxml \t parse a bxml file" << endl;
  cout << "--pog \t parse a pog file" << endl;
  cout << "--output\t <path> store the result in the given path. The file is "
          "displayed on stdout otherwise"
       << endl;
  cout << "--verbose \t print the execution time and the number of expressions "
          "parsed"
       << endl;
  cout << "--disable-multi-thread \t disable multi threading for pog files."
       << endl;
  cout << "--abstraction \t for pog files generated from abstract machines"
       << endl;
  cout << "--implementation \t for pog files generated from implementations"
       << endl;
  cout << "--help \t display the help menu" << endl;
}

void solve(genericparser::Parser::unique_ptr parser,
           genericwriter::Writer::unique_ptr writer, string input,
           string output, bool disable_multi_thread, bool verbose)
{
  Chrono chrono;
  chrono.start();
  XMLDocument doc;
  doc.LoadFile(input.c_str());
  XMLDocument *pDoc = doc.ToDocument();
  if (disable_multi_thread)
    parser->disableMultiThread();
  Context::shared_ptr context = parser->parse(pDoc);
  if (verbose)
    chrono.displayElapsedTime(TimeUnit::Seconds, "Parsing time : ");
  vector<Model::shared_ptr> models = context->getModels();
  const unordered_set<Expression::shared_ptr> expressions =
      context->getExpressions();
  chrono.reset();
  unordered_map<Variable::shared_ptr, string> var_to_type;

  try
  {
    ModelSet modelset = ModelSet(models);
    var_to_type = modelset.solve();
  }
  catch (SolverError e)
  {
    unordered_map<string, string> terms;
    for (auto expr : expressions)
      terms[expr->getAssociatedVariable()->toSMT()] =
          "t(" + expr->format() + ")";
    e.replaceTerms(terms);
    throw e;
  }
  if (verbose)
    chrono.displayElapsedTime(TimeUnit::Seconds, "Solving time : ");
  chrono.reset();
  if (output == "")
    writer->write(pDoc, expressions, var_to_type);
  else
  {
    writer->write(pDoc, output, expressions, var_to_type);
    if (verbose)
    {
      cout << "File " << output << " generated." << endl;
      chrono.displayElapsedTime(TimeUnit::Seconds, "Writing time : ");
    }
  }

  if (verbose)
    cout << "Number of expressions : " << expressions.size() << endl;

  // for (auto expression : expressions)
  // {
  //   Variable::shared_ptr var = expression->getAssociatedVariable();
  //   string type = var_to_type[var];
  //   cout << expression->format() << " : " << type << endl;
  // }
  // for (auto operation : context->getOperations())
  // {
  //   Variable::shared_ptr var = operation->getAssociatedVariable();
  //   string type = var_to_type[var];
  //   cout << operation->format() << " : " << type << endl;
  // }
}

int main(int argc, char **argv)
{
  const char *const short_opts = "";
  const struct option long_opts[] = {
      {"bxml", no_argument, nullptr, 'b'},
      {"pog", no_argument, nullptr, 'p'},
      {"output", required_argument, nullptr, 'o'},
      {"help", no_argument, nullptr, 'h'},
      {"verbose", no_argument, nullptr, 'v'},
      {"disable-multi-thread", no_argument, nullptr, 'd'},
      {"abstraction", no_argument, nullptr, 'a'},
      {"implementation", no_argument, nullptr, 'i'},
      {nullptr, no_argument, nullptr, 0}};

  bxml::Parser::unique_ptr bxml_parser;
  pog::Parser::unique_ptr pog_parser;
  Writer::unique_ptr writer = make_unique<Writer>();
  int opt;
  string output;
  vector<string> bxml_folders;
  bool pog = false, bxml = false;
  bool verbose = false;
  bool disable_multi_thread = false;
  genericparser::MachineType machine_type =
      genericparser::MachineType::Undefined;
  while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) !=
         -1)
  {
    switch (opt)
    {
    case 'b':
      bxml_parser = make_unique<bxml::Parser>();
      if (pog)
      {
        cerr << "File cannot be pog and bxml format at the same time";
        exit(1);
      }
      bxml = true;
      break;
    case 'p':
      pog_parser = make_unique<pog::Parser>();
      if (bxml)
      {
        cerr << "File cannot be pog and bxml format at the same time";
        exit(1);
      }
      pog = true;
      break;
    case 'o':
      output = optarg;
      break;
    case 'v':
      verbose = true;
      break;
    case 'd':
      disable_multi_thread = true;
      break;
    case 'a':
      machine_type = genericparser::MachineType::Abstraction;
      break;
    case 'i':
      machine_type = genericparser::MachineType::Implementation;
      break;
    case 'h':
      displayHelp(argv[0]);
      return 0;
    case '?':
      break;
    }
  }

  if (not bxml and not pog)
  {
    cerr << "The file to parse must be bxml or pog format" << endl;
    exit(1);
  }

  if ((argc - optind != 1 or argc < 2) and pog)
  {
    cerr << argv[0] << " takes exactly one argument for pog format" << endl;
    cerr << "Run " << argv[0] << " --help for more information" << endl;
    exit(1);
  }

  if ((argc - optind < 1 or argc < 2) and bxml)
  {
    cerr << argv[0] << " takes at least one argument for bxml format" << endl;
    cerr << "Run " << argv[0] << " --help for more information" << endl;
    exit(1);
  }

  string input = argv[optind];
  if (not filesystem::exists(input))
  {
    cerr << "File " << input << " not found." << endl;
    exit(1);
  }

  if (bxml)
  {
    if (machine_type != genericparser::MachineType::Undefined)
    {
      cerr << "Machine type should not be given as an argument for bxml files"
           << endl;
      exit(1);
    }
    if (disable_multi_thread)
    {
      cerr << "Multi threading is only available for pog files" << endl;
      exit(1);
    }

    for (unsigned int i = optind + 1; i < argc; i++)
      bxml_folders.emplace_back(argv[i]);
    bxml_parser->addFolders(bxml_folders);
    solve(move(bxml_parser), move(writer), input, output, disable_multi_thread,
          verbose);
  }
  if (pog)
  {
    if (machine_type == genericparser::MachineType::Undefined)
    {
      cerr << "Machine type has to be given." << endl;
      exit(1);
    }
    pog_parser->setMachineType(machine_type);
    solve(move(pog_parser), move(writer), input, output, disable_multi_thread,
          verbose);
  }

  return 0;
}
