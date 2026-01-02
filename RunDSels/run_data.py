#!/usr/bin/env python

import sys
import os
import subprocess
import time

# main function
def main():

        # option to turn on/off generated and phasespace trees creation
        make3kTrees = False
        make2pikpTrees = False
        make2pi0kpTrees = True
        make3pikpTrees = False

        # list of periods you'd like to analyze (used for input file path determination)
        period_names = ["2019_11"] #"2017_01", "2018_01", "2018_08"
        # list of initial integer in run number for given period (identifies correct input files)
        period_numbers = [7]#3,4,5 number with which run numbers start, e.g. for 2018-01 it is 4

        # Modify paths and filenames to match you reaction
        My3kDataPath = "/cache/halld/RunPeriod-2019-11/analysis/ver06/tree_kpkpkmlamb__B4_M18/merged/"
        My2pikpDataPath = "/cache/halld/RunPeriod-2019-11/analysis/ver06/tree_pippimkplamb__B4_M18/merged/"
        My2pi0kpDataPath = "/cache/halld/RunPeriod-2019-11/analysis/ver06/tree_pi0pi0kplamb__B4_M18/merged/"
        My3pikpDataPath = "/cache/halld/RunPeriod-2019-11/analysis/ver11/tree_pi0pippimkplamb__B4_M18/merged/"
        
        My3kTreeName = "kpkpkmlamb__B4_M18_Tree"
        My2pikpTreeName = "pippimkplamb__B4_M18_Tree"
        My2pi0kpTreeName = "pi0pi0kplamb__B4_M18_Tree"
        My3pikpTreeName = "pi0pippimkplamb__B4_M18_Tree"
	
        # loop over run periods to create all requested trees for each MC_ppippim_genamp_phasespace_3206/root/thrown/
        for period_name,period_number in zip(period_names,period_numbers):
        
            if make3kTrees:
                subprocess.call(["root.exe", "-l", "-b", "-q", '/work/halld/home/afzal/dselector/test/data_3k/runSelector_data.C("%02d","%s","%s", "/work/halld/home/afzal/dselector/test/DSelector_kpkpkmlamb.C", "kpkpkmlamb")' % (period_number, My3kDataPath, My3kTreeName)])
                #subprocess.call(["mv", "flattree_pippim.root", "flattree_pippim_SignalMCAcc_%s.root" % (period_name)])
            if make2pikpTrees:
                subprocess.call(["root.exe", "-l", "-b", "-q", '/work/halld/home/afzal/dselector/test/data_3k/runSelector_data.C("%02d","%s","%s", "/work/halld/home/afzal/dselector/test/DSelector_pippimkplamb.C", "pippimkplamb")' % (period_number, My2pikpDataPath, My2pikpTreeName)])
                #subprocess.call(["mv", "flattree_pippim.root", "flattree_pippim_SignalMCAcc_%s.root" % (period_name)])            
            if make2pi0kpTrees:
                subprocess.call(["root.exe", "-l", "-b", "-q", '/work/halld/home/afzal/dselector/test/data_3k/runSelector_data.C("%02d","%s","%s", "/work/halld/home/afzal/dselector/test/DSelector_pi0pi0kplamb.C", "pi0pi0kplamb")' % (period_number, My2pi0kpDataPath, My2pi0kpTreeName)])
                #subprocess.call(["mv", "flattree_pippim.root", "flattree_pippim_SignalMCAcc_%s.root" % (period_name)])
            if make3pikpTrees:
                subprocess.call(["root.exe", "-l", "-b", "-q", '/work/halld/home/afzal/dselector/test/data_3k/runSelector_data.C("%02d","%s","%s", "/work/halld/home/afzal/dselector/test/DSelector_pi0pippimkplamb.C", "pi0pippimkplamb")' % (period_number, My3pikpDataPath, My3pikpTreeName)])
                #subprocess.call(["mv", "flattree_pippim.root", "flattree_pippim_SignalMCAcc_%s.root" % (period_name)])  
            return

        return

if __name__=="__main__":
    main()
 
