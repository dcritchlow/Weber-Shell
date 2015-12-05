Feature: list command is processed properly
	Scenario: list command lists the current folder with decorations, one object per line
		Given a file named "listcurrent" with:
		"""
		list
		exit
		"""
		Given an empty file named "file666" with mode "666"
		Given an empty file named "file777" with mode "777"
		Given a directory named "dir777" with mode "777"
		When I run `../bin/wsh` interactively
		And I pipe in the file "listcurrent"
		And the stderr should not contain anything
		And the stdout should contain: 
		"""
		file666
		"""
		And the stdout should contain:
		"""
		file777*
		"""
		And the stdout should contain:
		"""
		dir777/
		"""
		Then 10 points are awarded

	Scenario: list foldera command lists foldera's contents only, with decorations, one object per line
		Given a directory named "folder/subfolder/subsubfolder"
		Given an empty file named "folder/file1" 
		Given an empty file named "folder/subfolder/file2"
		Given an empty file named "folder/subfolder/subsubfolder/file3"
		Given an empty file named "fileincurrentfolder" 
		And a file named "listfolder" with: 
		"""
		list folder
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "listfolder"
		Then the stderr should not contain anything
		And the stdout should contain: 
		"""
		folder/
		"""
		And the stdout should contain:
		"""
		subfolder/
		"""
		And the stdout should not contain:
		"""
		subsubfolder/
		"""
		And the stdout should contain:
		"""
		file1
		"""
		And the stdout should not contain:
		"""
		file2
		"""
		And the stdout should not contain:
		"""
		file3
		"""
		And the stdout should not contain:
		"""
		fileincurrentfolder
		"""
		Then 10 points are awarded

	Scenario: list -s command lists the current folder's contents and all subfolders, with decorations, one object per line
		Given a directory named "subfolder/subsubfolder"
		Given an empty file named "file1" 
		Given an empty file named "subfolder/file2"
		Given an empty file named "subfolder/subsubfolder/file3"
		Given an empty file named "fileincurrentfolder" 
		And a file named "listallfolder" with: 
		"""
		list -s
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "listallfolder"
		Then the stderr should not contain anything
		And the stdout should contain:
		"""
		subfolder/
		"""
		And the stdout should contain:
		"""
		subfolder:
		"""
		And the stdout should contain:
		"""
		subsubfolder/
		"""
		And the stdout should contain:
		"""
		subsubfolder:
		"""
		And the stdout should contain:
		"""
		file1
		"""
		And the stdout should contain:
		"""
		file2
		"""
		And the stdout should contain:
		"""
		file3
		"""
		And the stdout should contain:
		"""
		fileincurrentfolder
		"""
		Then 20 points are awarded

	Scenario: list -s folder command lists folder's contents and all subfolders, with decorations, one object per line
		Given a directory named "folder/subfolder/subsubfolder"
		Given a directory named "folder/subfolder/subsubfolder"
		Given an empty file named "folder/file1" 
		Given an empty file named "folder/subfolder/file2"
		Given an empty file named "folder/subfolder/subsubfolder/file3"
		Given an empty file named "fileincurrentfolder" 
		And a file named "listfolder" with: 
		"""
		list -s folder
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "listfolder"
		Then the stderr should not contain anything
		And the stdout should contain: 
		"""
		folder/
		"""
		And the stdout should contain: 
		"""
		folder:
		"""
		And the stdout should contain:
		"""
		subfolder/
		"""
		And the stdout should contain:
		"""
		subfolder:
		"""
		And the stdout should contain:
		"""
		subsubfolder/
		"""
		And the stdout should contain:
		"""
		subsubfolder:
		"""
		And the stdout should contain:
		"""
		file1
		"""
		And the stdout should contain:
		"""
		file2
		"""
		And the stdout should contain:
		"""
		file3
		"""
		And the stdout should not contain:
		"""
		fileincurrentfolder
		"""
		Then 20 points are awarded

