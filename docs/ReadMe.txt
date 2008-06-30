DivFix++ is complete rewrite of "DivFix" program due it's bugs and low performance.

DivFix++ designed to repair broken avi file streams by rebuilding index part of file.
This program is very useful when trying to preview movies which has no index part,
like some files are currently downloading at "eMuLe" (www.emule-project.org) and/or
Bittorrent network clients like azureus.

DivFix++ has CLI tools, this means you can fix, preview and delete movies
automaticly when using agruments... For Details, please look at Shell Options section.

Shell Options:
	-i, --input=<str>  	input file
	-o, --output=<str> 	output file or directory
	-p, --preview 	delete output file after player is closed
	-f, --fix_index_only 	doesn't cuts out bad parts of file
	-w, --overwrite 	overwrite original file
	-a, --all_frames 	doesn't skip unwanted frames
	-s, --skip_fix 	skips fix process if index is available
	-m, --media_player=<str> 	path of player
	-h, --help 	shows help

At preview mode, if file has proper index section, than program directly
executes video player (like VLC,mplayer or others...)

Emule Preview Automation:
With video players like VLC you can preview AVI files which has index part. But if avi file has not index part,seeking at file results locking programs in our example, VLC. DivFix++ has controls index section if its ok or not. If it's not, DivFix++ rebuilds the index part than calls executable.
That's why I have coded Shell options :)

For automatic preview from emule/amule you can directly give program name as video player name at Options -> Files -> Video Player

PS:
Emule,VLC and mplayer are registered trademarks.
For details, please look for:
www.emule-project.org
www.videolan.org
www.mplayerhq.hu
azureus.sourceforge.net
