#!/usr/bin/env python

import sys
from os.path import basename, splitext
from glob import glob

def Reformat(string):
  # First, remove the directory path
  string = basename(string)
  string = splitext(string)[0]
  newString = ""
  for c in string:
    if c == "_":
      newString = newString + "\\_"
    else:
      newString = newString + c
  
  return newString



StandardHeader = """\\documentclass{beamer}
  %\usetheme{default}
  \\usetheme{Madrid}
  \\usepackage{hyperref}
  \\xdefinecolor{lavender}{rgb}{0.8,0.6,1}
  \\xdefinecolor{olive}{cmyk}{0.64,0,0.95,0.4}
  \\newcommand{\signify}{\textcolor{purple}}
  %\\providecommand{\e}[1]{\ensuremath{\times 10^{#1}}}
  #\\begin{document}


  \\institute[UA]{
    University of California Irvine
    }
  \\author[C. Lampen]{Anyes Taffard}
  %\date{}

  \\title{Plots}

  \\begin{frame}[plain]
  \\titlepage
  \\end{frame}

  """

def picsToBeamer(patterns):
  """Dumps a beamer formatted string creating a presentation of pictures in alphabetical order
  """
  outstr = ""#StandardHeader

  #Take list of files, and find any globable patterns
  files =[]
  for pattern in patterns:
    files += glob(pattern)


  #make alphabetical
  files.sort()

  #print  "files for pattern: " + pattern
  #print files
  if len(files) == 0:
    print 'No files match pattern "' +pattern + '"'

  
  for f in files :
    reformatedFile = Reformat(f)
    outstr += "\\section{" + reformatedFile + "}\n"
    outstr += "\n\\begin{frame}{" + reformatedFile+ "}\n"
    outstr +="\\centerline{\n"
    #outstr += "\includegraphics[width=\\textwidth]{" + f + "}\n"
    outstr += "\includegraphics[width=0.7\\textwidth]{" + f + "}\n"
    outstr +="}\n"
    outstr += "\end{frame}\n"

  #outstr +="\n\\end{document}"
  return outstr
#--End picsToBeamer1
  



if __name__ == "__main__":

  argv = sys.argv

  version = '1'
  if(len(argv)==1):
    print "usage: " 
    print argv[0] + " inputPattern(s)"
  pattern = ""

  patterns = argv[1:]
 
  #if(len(argv) > 2):
  #outFileName = argv[2]
  #else:
  #  outFileName = "plots.tex"

  outStr = picsToBeamer(patterns)

  print outStr
  #out = open(outFileName, 'w')
  #out.write(outstr)

