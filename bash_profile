# .bash_profile

# Get the aliases and functions
#if [ -f ~/.bashrc ]; then
#	. ~/.bashrc
#fi

# User specific environment and startup programs

PATH=$PATH:$HOME/bin


export PATH
unset USERNAME
if [ -z "$LOGNAME" ]; then
        export LOGNAME=`logname`    # name of user who logged in
fi
export LANG=C;

export DB_TIMEOUT=5

#sh PATH
export ORACLE_BASE=/home/oracle/app/oracle
export ORACLE_HOME=/home/oracle/app/oracle/product/10.2.0/db_1
export ORACLE_SID=pdborcl
export PATH=/usr/bin:/etc:/usr/sbin:/usr/local/bin:/bin:/sbin
export PATH=$PATH:/usr/ucb:/usr/bin/X11:/sbin:/usr/DynamicLinkManager/bin
export PATH=$PATH:/usr/vac/bin:/home/FortifySoftware/SCAS-EE4.5.0
export PATH=$PATH:/opt/rational/clearcase/bin
export PATH=$PATH:$ORACLE_HOME/bin:.
export PATH=$PATH:$HOME/tsp30/Shells
export BASESERV=pdborcl
export BASESERVBUSI=pdborcl
# For Mon database
export BASEMONSERV=oracle
export BASEMONUSER=orcl
export BASEMONPASS=orcl

# For dynamic library
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/lib30:$HOME/ulib30:$HOME/local/lib:/lib:/lib/noelision:/lib/security:/lib64:/lib64/multipath:/lib64/noelision:/lib64/security    #alter ly 20170513
export LIBPATH=$LD_LIBRARY_PATH

#for MSP
MSP_NODE_ID=2
MSP_DIR=$HOME
MSP_PORT=6500
MSP_BUS_ID=2
MSP_GROUP_ID=2
PATH=$PATH:$MSP_DIR/bin
export MSP_NODE_ID MSP_DIR MSP_PORT MSP_BUS_ID MSP_GROUP_ID PATH

# BAL
export TSP_COMBAL_ID=2
export BRANCH_CODE=210000000

#add by ly 20170508
export API_LIB_PATH=$HOME/lib30
export API_INCL_PATH=$HOME/inc30
export SUNLINK_LIB_PATH=$HOME/lib30:$HOME/inc30

set -o vi
alias rmlog='rm -fr $HOME/log/*.*'
alias rm='rm -i'
alias cdlog='cd $HOME/log'
alias psw='ps -ef|grep $LOGNAME'
alias ll='ls -lrt'
alias l='ls -lrt'
alias cdshell='cd $HOME/Shells'
alias cdtm='cd $HOME/MonSys'
alias cdxml='cd $HOME/libxml'

stty erase ^H

