# If you come from bash you might have to change your $PATH.
# export PATH=$HOME/bin:/usr/local/bin:$PATH
# Path to your oh-my-zsh installation.  export ZSH=/Users/lambda/.oh-my-zsh

# Set name of the theme to load. Optionally, if you set this to "random"
# it'll load a random theme each time that oh-my-zsh is loaded.  # See https://github.com/robbyrussell/oh-my-zsh/wiki/Themes
#ZSH_THEME="robbyrussell"
ZSH_THEME="agnoster"  #使用 agnoster 主题，很好很强大
DEFAULT_USER="lambda"     #增加这一项，可以隐藏掉路径前面那串用户名
plugins=(git brew node npm)   #自己按需把要用的 plugin 写上
# Uncomment the following line to use case-sensitive completion.
# CASE_SENSITIVE="true"

# Uncomment the following line to use hyphen-insensitive completion. Case
# sensitive completion must be off. _ and - will be interchangeable.
# HYPHEN_INSENSITIVE="true"

# Uncomment the following line to disable bi-weekly auto-update checks.
# DISABLE_AUTO_UPDATE="true"

# Uncomment the following line to change how often to auto-update (in days).
# export UPDATE_ZSH_DAYS=13

# Uncomment the following line to disable colors in ls.
# DISABLE_LS_COLORS="true"

# Uncomment the following line to disable auto-setting terminal title.
# DISABLE_AUTO_TITLE="true"

# Uncomment the following line to enable command auto-correction.
# ENABLE_CORRECTION="true"

# Uncomment the following line to display red dots whilst waiting for completion.
# COMPLETION_WAITING_DOTS="true"

# Uncomment the following line if you want to disable marking untracked files
# under VCS as dirty. This makes repository status check for large repositories
# much, much faster.
# DISABLE_UNTRACKED_FILES_DIRTY="true"

# Uncomment the following line if you want to change the command execution time
# stamp shown in the history command output.
# The optional three formats: "mm/dd/yyyy"|"dd.mm.yyyy"|"yyyy-mm-dd"
# HIST_STAMPS="mm/dd/yyyy"

# Would you like to use another custom folder than $ZSH/custom?
# ZSH_CUSTOM=/path/to/new-custom-folder

# Which plugins would you like to load? (plugins can be found in ~/.oh-my-zsh/plugins/*)
# Custom plugins may be added to ~/.oh-my-zsh/custom/plugins/
# Example format: plugins=(rails git textmate ruby lighthouse)
# Add wisely, as too many plugins slow down shell startup.
plugins=(git)

export MVIM=/Applications/MacVim.app/Contents/bin
export EMAC=/Applications/Emacs.app/Contents/MacOS
export ZSH=/Users/lambda/.oh-my-zsh
export HOSTCONN=/Users/lambda/.host_conn
export PYSITE37=/Users/lambda/Library/Python/3.7/bin
export OPENSSL=/usr/local/opt/openssl/bin
export LLVM=/usr/local/opt/llvm/bin
export GTEXT=/usr/local/opt/gettext/bin
export PATH=$HOSTCONN:$PYSITE37:$MVIM:$OPENSSL:$GTEXT:$PATH
export MYDEV=~/Develop/code/MyProject
source $ZSH/oh-my-zsh.sh

# User configuration

export MANPATH="/usr/local/man:$MANPATH"

# You may need to manually set your language environment
export LANG=en_US.UTF-8

# Preferred editor for local and remote sessions
if [[ -n $SSH_CONNECTION ]]; then
  export EDITOR='vim'
else
  export EDITOR='vim'
fi

# Compilation flags
# export ARCHFLAGS="-arch x86_64"

# ssh
export SSH_KEY_PATH="~/.ssh/rsa_id"

# some function
#######################################
# Call virtualenv to create a venv
# Global
#     MYDEV
# Arguments:
#     venv's dir name
#     if not given, mkenv with dir name
# Returns:
#     None
######################################
function SetPyVenv() {
    local curr_dir=$(pwd)
    if [[ $# == 1 ]]
    then
        local venv_path=${MYDEV}/src/py/$1
        mkdir -p ${venv_path}
        cd ${venv_path}
    fi
    local venv_name=$(basename $(pwd))_venv
    if [[ ! -d ./${venv_name} ]]
    then
        virtualenv ${venv_name}
    fi
    cd ${curr_dir}
}

#######################################
# Activate virtual env
# Global
#     MYDEV
# Arguments:
#     venv's dir name
#     if not given, activate cwd venv
# Returns:
#     None
######################################
function ActivatePyVenv() {
    if [[ $# == 1 ]]
    then
        local venv_path=${MYDEV}/src/py/$1
        if [[ -d ${venv_path} ]]
        then
            cd ${venv_path}
        else
            return
        fi
    fi
    local venv_name=$(basename $(pwd))_venv
    source ${venv_name}/bin/activate.sh
    export VENV=$(pwd)
}

#######################################
# Deactivate current virtual env
# Arguments:
#     None
# Returns:
#     None
######################################
function DeactivatePyVenv() {
    local venv_path=$(dirname $(which python))/../..
    pip freeze > ${venv_path}/requirements.txt
    deactivate
    export VENV=""
}

#######################################
# Remove a specific virtual env
#     MYDEV
# Arguments:
#     venv's dir name
#     if not given, remove cwd venv
# Returns:
#     None
######################################
function RemovePyVenv() {
    local curr_dir=$(pwd)
    if [[ $# == 1 ]]
    then
        local venv_path=${MYDEV}/src/py/$1
        if [[ -d ${venv_path} ]]
        then
            cd ${venv_path}
        else
            return
        fi
    fi
    local venv_name=$(basename $(pwd))_venv
    rm -rf ${venv_name}
    cd ${curr_dir}
}

# Set personal aliases, overriding those provided by oh-my-zsh libs,
# plugins, and themes. Aliases can be placed here, though oh-my-zsh
# users are encouraged to define aliases within the ZSH_CUSTOM folder.
# For a full list of active aliases, run `alias`.
#
# Example aliases
# alias zshconfig="mate ~/.zshrc"
# alias ohmyzsh="mate ~/.oh-my-zsh"

# general alias
alias ep="vim ~/.zshrc"
alias sp="source ~/.zshrc"
alias zshupate="upgrade_oh_my_zsh"
alias vi="vim"
alias ase="/Users/lambda/Library/ApplicationSupport/Steam/steamapps/common/Aseprite/Aseprite.app/Contents/MacOS/aseprite"
alias asef="ase -b --filename-format '{title}_{frame001}.{extension}'"

# git alias
alias gr="git reset --hard HEAD"
alias gd="git diff | cdiff -s"
alias gcd="git diff --cached | cdiff -s"
alias gs="git status -bs"
alias gl="git log --graph --decorate --oneline --all"

# eisoo alias
alias cddev="cd ~/Develop/code/MyProject/src"
alias cdtool="cd ~/Develop/code/MyProject/cmake/tool"
alias cdrun="cd ~/Develop/code/MyProject/target/Darwin_17_x64/libs/Debug"
alias se="source initEnv.sh"
alias m="makec debug"
alias mr="makec release"
alias mc="makec debug cleanall"
alias mrc="makec release cleanall"

# ssh alias
alias sshc7="ssh root@192.168.53.156"
alias sshse="ssh root@192.168.244.124"

# pyenv alias
alias mkenv="SetPyVenv"
alias workon="ActivatePyVenv"
alias leaveoff="DeactivatePyVenv"
alias rmenv="RemovePyVenv"
