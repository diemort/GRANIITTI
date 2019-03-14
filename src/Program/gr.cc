// GRANIITTI - Monte Carlo event generator for high energy diffraction
// https://github.com/mieskolainen/graniitti
//
// <Main generator program>
//
// (c) 2017-2019 Mikael Mieskolainen
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.


// C++
#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>

// Own
#include "Graniitti/MAux.h"
#include "Graniitti/MGraniitti.h"
#include "Graniitti/MMatrix.h"

// Libraries
#include "json.hpp"
#include "rang.hpp"
#include "cxxopts.hpp"

using namespace gra;

// Main
int main(int argc, char* argv[]) {
	
    // Save the number of input arguments
	const int NARGC = argc - 1;
	
	// Create generator object first
	std::unique_ptr<MGraniitti> gen = std::make_unique<MGraniitti>();
	
    try {
	cxxopts::Options options(argv[0], "");

	options.add_options("")
		("i,INPUT",    "Input card",                     cxxopts::value<std::string>() )
  		("H,help",     "Help")
  		;
	
	options.add_options("GENERALPARAM")
  		("o,OUTPUT",     "Output name",                  cxxopts::value<std::string>() )
  		("f,FORMAT",     "Output format",                cxxopts::value<std::string>() )
  		("n,NEVENT",     "Number of events",             cxxopts::value<unsigned int>() )
  		("g,INTEGRATOR", "Integrator",                   cxxopts::value<std::string>() )
  		("w,WEIGHTED",   "Weighted events (true/false)", cxxopts::value<std::string>() )
  		("c,CORES",      "Number of CPU cores/threads",  cxxopts::value<unsigned int>() )
  		;
  		
 	options.add_options("PROCESSPARAM")
 		("p,PROCESS",  "Process",                cxxopts::value<std::string>() )
  		("l,POMLOOP",  "Screening Pomeron loop", cxxopts::value<std::string>() )
	  	("s,NSTARS",   "Excite protons (0,1,2)", cxxopts::value<unsigned int>() )
	  	("q,LHAPDF",   "Set LHAPDF",             cxxopts::value<std::string>() )
	  	("h,HIST",     "Histogramming",          cxxopts::value<unsigned int>() )
	  	("a,FLATAMP",  "Flat matrix element",    cxxopts::value<unsigned int>() )
	  	("r,RNDSEED",  "Random seed",            cxxopts::value<unsigned int>() )
  		;
		
	auto r = options.parse(argc, argv);
	
	if (r.count("help") || NARGC == 0) {
		gen->GetProcessNumbers();
		std::cout << options.help({"", "GENERALPARAM", "PROCESSPARAM"}) << std::endl;
		std::cout << "Arrow operators:" << std::endl;
		std::cout << "  use -> between initial and final states" << std::endl;
		std::cout << "  use &> (instead of ->) for a decoupled central system phase space (use with <F> class, e.g. for s-channel resonances)" << std::endl;
		std::cout << "  use  > for recursive decaytree branchings with curly brackets { grand daughters }" << std::endl;
		std::cout << std::endl;
		std::cout << "PROCESS examples:" << std::endl;
		std::cout << "  yy[CON]<C> -> mu+ mu-" << std::endl;
		std::cout << "  PP[CON]<C> -> rho(770)0 > {pi+ pi-} rho(770)0 > {pi+ pi-}" << std::endl;
		std::cout << std::endl;
		std::cout << "A steering card example with no additional input:" << std::endl;
		std::cout << "  " << argv[0] << " -i ./input/test.json" << std::endl << std::endl;
		
		return EXIT_FAILURE;
	}

	// -------------------------------------------------------------------
	// This first
	// Process re-set from commandline
	if (r.count("p"))  {
		gen->ReadInput(r["i"].as<std::string>(), r["p"].as<std::string>());
	} else {
		gen->ReadInput(r["i"].as<std::string>());		
	}
	
	// -------------------------------------------------------------------
	// Quick custom parameters
	
	// General parameters (order is important)
	if (r.count("n"))   { gen->SetNumberOfEvents(r["n"].as<unsigned int>()); }
	if (r.count("o") && r.count("f"))   {
		gen->SetOutput(r["o"].as<std::string>());
		gen->SetFormat(r["f"].as<std::string>());	
	}
	else if ((r.count("o") && !r.count("f")) || (!r.count("o") && r.count("f")) ) {
		throw std::invalid_argument("Error: Use options -o and -f together.");
	}
	if (r.count("g"))   {
		const std::string val = r["g"].as<std::string>();
		gen->SetIntegrator(val);
	}
	if (r.count("w"))   { 
		const std::string val = r["w"].as<std::string>();
		gen->SetWeighted(val == "true");
	}
	if (r.count("c"))   { gen->SetCores(r["c"].as<unsigned int>()); }
	
	// Process parameters (adding more might be involved due to initialization in MProcess...)
	if (r.count("l"))  { 
		const std::string val = r["l"].as<std::string>();
		gen->proc->SetScreening(val == "true");
	}
	
	if (r.count("s"))  { gen->proc->SetExcitation(r["s"].as<unsigned int>()); }
	if (r.count("q"))  { gen->proc->SetLHAPDF(r["q"].as<std::string>()); }
	if (r.count("h"))  { gen->proc->SetHistograms(r["h"].as<unsigned int>()); }
	if (r.count("a"))  { gen->proc->SetFlatAmp(r["a"].as<unsigned int>()); }
	if (r.count("r"))  { gen->proc->random.SetSeed(r["r"].as<unsigned int>()); }
	
	// -------------------------------------------------------------------

	// Initialize generator (always last!)
	gen->Initialize();

	// Generate events
	gen->Generate();

	// Print histograms
	gen->PrintHistograms();
	
    } catch (const std::invalid_argument& e) {
		gen->GetProcessNumbers();
		gra::aux::PrintGameOver();
		std::cerr << rang::fg::red
		          << "Exception catched: " << rang::fg::reset << e.what()
		          << std::endl;
		return EXIT_FAILURE;
    } catch (const std::ios_base::failure& e) {
		gra::aux::PrintGameOver();
		std::cerr << rang::fg::red
		          << "Exception catched: std::ios_base::failure: "
		          << rang::fg::reset << e.what() << std::endl;
		return EXIT_FAILURE;
	} catch (const cxxopts::OptionException& e) {
	    gra::aux::PrintGameOver();
	    std::cerr << rang::fg::red
	              << "Exception catched: Commandline options: " << rang::fg::reset << e.what() << std::endl;
	    return EXIT_FAILURE;
	} catch (...) {
		gen->GetProcessNumbers();
		gra::aux::PrintGameOver();
		std::cerr << rang::fg::red << "Exception catched: Unspecified (...) (Probably JSON input)" << rang::fg::reset
		          << std::endl;
		return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
