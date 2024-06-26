/*
 * A TYPe Inference Kit for B.
 *
 * This file is part of the atypik project.
 * Copyright (c) 2023 CLEARSY
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License version 3
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>. 
 *
 * You can be released from the requirements of the license by purchasing
 * a commercial license. Buying such a license is mandatory as soon as you
 * develop commercial activities involving the atypik software without
 * disclosing the source code of your own applications.
 *
 */
#include <getopt.h>

#include <algorithm>
#include <execution>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "bxmlparser.h"
#include "csvwriter.h"
#include "error.h"
#include "modelset.h"
#include "pogparser.h"
#include "solverfactory.h"
#include "writer.h"

using namespace belem;
using namespace solver;
using namespace std;
using namespace test;
using namespace tinyxml2;
using genericparser::Context;
using genericwriter::Writer;

void displayHelp(string prog) {
  cout << "usage " << prog << " <file.(bxml)|(pog)> folders_to_bxml_files"
       << endl;
  cout << "Options:" << endl;
  cout << "--bxml \t parse a bxml file" << endl;
  cout << "--pog \t parse a pog file" << endl;
  cout << "--output\t <path> store the result in the given path. The file is "
          "displayed on stdout otherwise"
       << endl;
  cout << "--help \t display the help menu" << endl;
}

void write(genericparser::Parser::unique_ptr parser,
           genericwriter::Writer::unique_ptr writer, string input,
           string output) {
  XMLDocument doc;
  doc.LoadFile(input.c_str());
  XMLDocument* pDoc = doc.ToDocument();
  Context::shared_ptr context = parser->parse(pDoc);
  vector<Model::shared_ptr> models = context->getModels();
  const unordered_set<Expression::shared_ptr> expressions =
      context->getExpressions();
  unordered_map<Variable::shared_ptr, string> var_to_type;
  try {
    ModelSet modelset = ModelSet(models);
    var_to_type = modelset.solve();
  } catch (SolverError) {
    cerr << "No template could be generated for unsatisfiable models" << endl;
    exit(1);
  }
  CSVWriter::writeTemplate(expressions, var_to_type, output, "\t");
}

int main(int argc, char** argv) {
  const char* const short_opts = "";
  const struct option long_opts[] = {
      {"bxml", no_argument, nullptr, 'b'},
      {"pog", no_argument, nullptr, 'p'},
      {"output", required_argument, nullptr, 'o'},
      {"help", no_argument, nullptr, 'h'},
      {nullptr, no_argument, nullptr, 0}};

  bxml::Parser::unique_ptr bxml_parser;
  pog::Parser::unique_ptr pog_parser;
  Writer::unique_ptr writer = make_unique<Writer>();
  int opt;
  string output;
  vector<string> bxml_folders;
  bool pog = false, bxml = false;
  while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) !=
         -1) {
    switch (opt) {
      case 'b':
        bxml_parser = make_unique<bxml::Parser>();
        if (pog) {
          cerr << "File cannot be pog and bxml format at the same time";
          exit(1);
        }
        bxml = true;
        break;
      case 'p':
        pog_parser = make_unique<pog::Parser>();
        if (bxml) {
          cerr << "File cannot be pog and bxml format at the same time";
          exit(1);
        }
        pog = true;
        break;
      case 'o':
        output = optarg;
        break;
      case 'h':
        displayHelp(argv[0]);
        return 0;
      case '?':
        break;
    }
  }

  if (not bxml and not pog) {
    cerr << "The file to parse must be bxml or pog format" << endl;
    exit(1);
  }

  if ((argc - optind != 1 or argc < 2) and pog) {
    cerr << argv[0] << " takes exactly one argument for pog format" << endl;
    cerr << "Run " << argv[0] << " --help for more information" << endl;
    exit(1);
  }

  if ((argc - optind < 1 or argc < 2) and bxml) {
    cerr << argv[0] << " takes at least one argument for bxml format" << endl;
    cerr << "Run " << argv[0] << " --help for more information" << endl;
    exit(1);
  }

  string input = argv[optind];
  if (not filesystem::exists(input)) {
    cerr << "File " << input << " not found." << endl;
    exit(1);
  }

  if (bxml) {
    for (unsigned int i = optind + 1; i < argc; i++)
      bxml_folders.emplace_back(argv[i]);
    bxml_parser->addFolders(bxml_folders);
    write(move(bxml_parser), move(writer), input, output);
  }
  if (pog) write(move(pog_parser), move(writer), input, output);

  return 0;
}
