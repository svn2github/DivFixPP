DivFix++ is complete rewrite of "DivFix" program due it's bugs and low performance.

DivFix++ designed to repair broken avi file streams by rebuilding index part of file.
This program is very useful when trying to preview movies which has no index part,
like some files are currently downloading at "eMuLe" (www.emule-project.org)...

DivFix++ has CLI tools, this means you can fix, preview and delete movies
automaticly when using agruments... For Details, please look at Shell Options section.

Shell Options:
  -h, --help            shows help
  -nc, --no_cut_out     do not cut out bad parts of file
  -nk, --no_keep        overwrites original file
  -ns, --no_skip........do not skip unwanted frames
  -o, --output=<str>    output file or directory
  -i, --input=<str>     input file
  -p, --preview         deletes output file after player is closed
  -s, --skip_fix        skips fix process if index is available
  -e, --execute=<str>   executes player with input file after fix


At preview mode, if file has proper index section, than program directly
executes video player (like VLC, please look at www.videolan.org).

Emule Preview Automation:

With VLC you can preview AVI files which has index part. But if avi file has not index
part,seeking at file results locking VLC. DivFix++ has controls index section if its
ok or not. If it's not, DivFix++ rebuilds the index part than calls executable.
That's why I have coded Shell options :)

For automatic preview from emule, you have to write some BATCH script like that...
Create a text file... (like preview.txt)
and write :

start "<DivFix++ Path>" -p -e "<VLC path>" -i %1

save this file and rename file to preview.bat
<DivFix Path> means full path of DivFix++ like C:\Program Files\DivFix++\DivFix.exe

a working command line looks like this for me:

start "X:\C++\Project\DivFix++\bin\DivFix++.exe" -p -e "X:\Utility\vlc\vlc.exe" -i %1

after all, you have to change emule's preview program...

At Options -> Files -> Video Player
You have to write your "preview.bat" files full path, like:

X:\Utility\MorphXT\preview.bat

Don't try "Browse" metot, emule doesn't support this.

PS:
Emule and VLC are registered trademarks.
For details, please look for: www.emule-project.org & www.videolan.org
