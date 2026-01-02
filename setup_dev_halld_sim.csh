
module use /usr/share/modulefiles
module load mpi/openmpi-x86_64
source /group/halld/Software/build_scripts/gluex_env_jlab.csh /work/halld/home/ilbelov/version_6.3.0_loc_halld_sim.xml 

setenv PATH $ROOTSYS/bin:$PATH
setenv AMPTOOLS $AMPTOOLS_HOME/AmpTools

setenv FSROOT /work/halld/home/ilbelov/FSRoot
#setenv DYLD_LIBRARY_PATH $DYLD_LIBRARY_PATH\:$FSROOT
#setenv LD_LIBRARY_PATH $LD_LIBRARY_PATH\:$FSROOT
