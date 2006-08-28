#!/bin/sh
# the next line restarts using tclsh \
    exec tclsh "$0" "$@"

################################################################################
#
# getbuildtest.tcl
#
# does an update and a clean build of slicer3 (including utilities and libs)
# then does a dashboard submission
#
# Usage:
#   getbuildtest [options] [target]
#
# Initiated - sp - 2006-05-11
#

################################################################################
#
# simple command line argument parsing
#

proc Usage { {msg ""} } {
    global SLICER
    
    set msg "$msg\nusage: getbuildtest \[options\] \[target\]"
    set msg "$msg\n  \[target\] is determined automatically if not specified"
    set msg "$msg\n  \[options\] is one of the following:"
    set msg "$msg\n   h --help : prints this message and exits"
    set msg "$msg\n   -f --clean : delete lib and build directories first"
    set msg "$msg\n   -t --test-type : CTest test target"
    set msg "$msg\n   --release : compile with optimization flags"
    set msg "$msg\n   -u --update : does a cvs/svn update on each lib"
    puts stderr $msg
}

set GETBUILDTEST(clean) "false"
set GETBUILDTEST(update) ""
set GETBUILDTEST(release) ""
set GETBUILDTEST(test-type) "Experimental"
set strippedargs ""
set argc [llength $argv]
for {set i 0} {$i < $argc} {incr i} {
    set a [lindex $argv $i]
    switch -glob -- $a {
        "--clean" -
        "-f" {
            set GETBUILDTEST(clean) "true"
        }
        "--update" -
        "-u" {
            set GETBUILDTEST(update) "--update"
        }
        "--release" {
            set GETBUILDTEST(release) "--release"
        }
             "-t" -
        "--test-type" {
                incr i
                if { $i == $argc } {
                    Usage "Missing test-type argument"
                } else {
                    set GETBUILDTEST(test-type) [lindex $argv $i]
                }
        }
        "--help" -
        "-h" {
            Usage
            exit 1
        }
        "-*" {
            Usage "unknown option $a\n"
            exit 1
        }
        default {
            lappend strippedargs $a
        }
    }
}
set argv $strippedargs
set argc [llength $argv]

if {$argc > 1 } {
    Usage
    exit 1
}


################################################################################
#
# Utilities:

proc runcmd {args} {
    global isWindows
    puts "running: $args"

    # print the results line by line to provide feedback during long builds
    # interleaves the results of stdout and stderr, except on Windows
    if { $isWindows } {
        # Windows does not provide native support for cat
        set fp [open "| $args" "r"]
    } else {
        set fp [open "| $args |& cat" "r"]
    }
    while { ![eof $fp] } {
        gets $fp line
        puts $line
    }
    set ret [catch "close $fp" res] 
    if { $ret } {
        puts stderr $res
        if { $isWindows } {
            # Does not work on Windows
        } else {
            error $ret
        }
    } 
}


################################################################################
# First, set up the directory
# - determine the location
# - determine the build
# 

set script [info script]
catch {set script [file normalize $script]}
set ::SLICER_HOME [file dirname [file dirname $script]]
set cwd [pwd]
cd [file dirname [info script]]
cd ..
set ::SLICER_HOME [pwd]
cd $cwd

set ::SLICER_LIB $::SLICER_HOME/../Slicer3-lib
set ::SLICER_BUILD $::SLICER_HOME/../Slicer3-build

#######
#
# Note: the local vars file, slicer2/slicer_variables.tcl, overrides the default values in this script
# - use it to set your local environment and then your change won't 
#   be overwritten when this file is updated
#
set localvarsfile $SLICER_HOME/slicer_variables.tcl
catch {set localvarsfile [file normalize $localvarsfile]}
if { [file exists $localvarsfile] } {
    puts "Sourcing $localvarsfile"
    source $localvarsfile
} else {
    puts "stderr: $localvarsfile not found - use this file to set up your build"
    exit 1
}

#initialize platform variables
foreach v { isSolaris isWindows isDarwin isLinux } { set $v 0 }
switch $tcl_platform(os) {
    "SunOS" { set isSolaris 1 }
    "Linux" { set isLinux 1 }
    "Darwin" { set isDarwin 1 }
    default { set isWindows 1 }
}

#
# Deletes both SLICER_LIB and SLICER_BUILD if clean option given
#
# tcl file delete is broken on Darwin, so use rm -rf instead
if { $GETBUILDTEST(clean) } {
    puts "Deleting slicer lib files..."
    if { $isDarwin } {
        runcmd rm -rf $SLICER_LIB
        runcmd rm -rf $SLICER_BUILD
        if { [file exists $SLICER_HOME/isPatched] } {
            runcmd rm $SLICER_HOME/isPatched
        }

        if { [file exists $SLICER_HOME/isPatchedBLT] } {
            runcmd rm $SLICER_HOME/isPatchedBLT
        }
    } else {
        file delete -force $SLICER_LIB
        file delete -force $SLICER_BUILD
    }
}

if { ![file exists $SLICER_LIB] } {
    file mkdir $SLICER_LIB
}

if { ![file exists $SLICER_BUILD] } {
    file mkdir $SLICER_BUILD
}


################################################################################
#
# the actual build and test commands
# - checkout the source code
# - make the prerequisite libs
# - cmake and build the program
# - run the tests
# - make a package
#


# svn checkout (does an update if it already exists)
cd $::SLICER_HOME/..
runcmd svn checkout http://www.na-mic.org:8000/svn/Slicer3/trunk Slicer3

# build the lib with options
cd $::SLICER_HOME
set cmd "sh $::SLICER_HOME/Scripts/genlib.tcl $SLICER_LIB"
if { $::GETBUILDTEST(release) != "" } {
   append cmd " $::GETBUILDTEST(release)"
} 
if { $::GETBUILDTEST(update) != "" } {
   append cmd " $::GETBUILDTEST(update)"
} 
eval runcmd $cmd


# build the slicer
cd $::SLICER_BUILD
runcmd $::CMAKE \
        -G$::GENERATOR \
        -DITK_DIR:FILEPATH=$ITK_BINARY_PATH \
        -DKWWidgets_DIR:FILEPATH=$SLICER_LIB/KWWidgets-build \
        $SLICER_HOME

if { $isWindows } {
    if { $MSVC6 } {
        eval runcmd $::MAKE Slicer3.dsw /MAKE $::GETBUILDTEST(test-type)
        eval runcmd $::MAKE Slicer3.dsw /MAKE package
    } else {
        runcmd $::MAKE Slicer3.SLN /build $::VTK_BUILD_TYPE $::GETBUILDTEST(test-type)
        runcmd $::MAKE Slicer3.SLN /build $::VTK_BUILD_TYPE package
    }
} else {
    set buildReturn [catch "eval runcmd $::MAKE $::GETBUILDTEST(test-type)"]
    set packageReturn [catch "eval runcmd $::MAKE package"]

    puts "\nResults: "
    puts "build [if $buildReturn "concat failed" "concat succeeded"]"
    puts "package [if $packageReturn "concat failed" "concat succeeded"]"
}
