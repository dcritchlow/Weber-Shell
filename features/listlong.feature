Feature: list commands with -l and -s are processed properly
	Scenario: list -l command lists the current folder with mode bits, size, date, time and name, one object per line
		Given a file named "listlongcurrent" with:
		"""
		list -l
		exit
		"""
		Given a 2000 byte file named "file2000" with mode "644" and date "201412250700.00"
		Given a 3000 byte file named "file3000" with mode "711" and date "201512250800.00"
		Given a directory named "smallsubdir" with mode "732" and date "201612250900.00"
		When I run `../bin/wsh` interactively
		And I pipe in the file "listlongcurrent"
		And the stderr should not contain anything
		Then the output should match:
		"""
		-rw-r--r--\s+2000\s+12/25/2014\s+07:00:00\s+file2000
		"""
		Then the output should match:
		"""
		-rwx--x--x\s+3000\s+12/25/2015\s+08:00:00\s+file3000
		"""
		Then the output should match:
		"""
		drwx-wx-w-\s+4096\s+12/25/2016\s+09:00:00\s+smallsubdir
		"""
		Then 15 points are awarded

	Scenario: list -l folderl command lists folderl's contents with mode bits, size, date, time and name, one object per line
		Given a 4000 byte file named "folderl/file4000" with mode "760" and date "201612250700.16"
		Given a 5000 byte file named "folderl/file5000" with mode "123" and date "201712250700.17"
		Given an empty file named "filerighthere"
		And a file named "listlongfolder" with: 
		"""
		list -l folderl
		exit
		"""
		Given a directory named "folderl/subfolder/subsubfolder" with mode "733" and date "201502140600.00"
		Given a directory named "folderl/subfolder" with mode "721" and date "201402140600.00"
		Given a directory named "folderl" with mode "712" and date "201302140600.00"
		When I run `../bin/wsh` interactively
		And I pipe in the file "listlongfolder"
		Then the stderr should not contain anything
		Then the output should match:
		"""
		-rwxrw----\s+4000\s+12/25/2016\s+07:00:16\s+file4000
		"""
		Then the output should match:
		"""
		---x-w--wx\s+5000\s+12/25/2017\s+07:00:17\s+file5000
		"""
		Then the output should match:
		"""
		drwx-w---x\s+4096\s+02/14/2014\s+06:00:00\s+subfolder
		"""
		And the output should not contain "filerighthere"
		And the output should not contain "subsubfolder"
		Then 15 points are awarded

	Scenario: list -ls command lists the current folders' contents and all subfolders,  with mode bits, size, date, time and name, one object per line
		Given a 4000 byte file named "folderl/file4000" with mode "760" and date "201612250700.16"
		Given a 5000 byte file named "folderl/file5000" with mode "123" and date "201712250700.17"
		Given a 6000 byte file named "filerighthere" with mode "456" and date "201812250700.18"
		And a file named "listlonganddeep" with: 
		"""
		list -ls 
		exit
		"""
		Given a directory named "folderl/subfolder/subsubfolder" with mode "733" and date "201502140600.01"
		Given a directory named "folderl/subfolder" with mode "721" and date "201402140600.00"
		Given a directory named "folderl" with mode "712" and date "201302140600.00"
		When I run `../bin/wsh` interactively
		And I pipe in the file "listlonganddeep"
		Then the stderr should not contain anything
		Then the output should match:
		"""
		-rwxrw----\s+4000\s+12/25/2016\s+07:00:16\s+file4000
		"""
		Then the output should match:
		"""
		---x-w--wx\s+5000\s+12/25/2017\s+07:00:17\s+file5000
		"""
		Then the output should match:
		"""
		-r--r-xrw-\s+6000\s+12/25/2018\s+07:00:18\s+filerighthere
		"""
		Then the output should match:
		"""
		drwx-w---x\s+4096\s+02/14/2014\s+06:00:00\s+subfolder
		"""
		Then the output should match:
		"""
		drwx-wx-wx\s+4096\s+02/14/2015\s+06:00:01\s+subsubfolder
		"""
		Then 20 points are awarded

	Scenario: list -ls folders command lists folders' contents and all subfolders,  with mode bits, size, date, time and name, one object per line
		Given a 4000 byte file named "folderl/file4000" with mode "760" and date "201612250700.16"
		Given a 5000 byte file named "folderl/file5000" with mode "123" and date "201712250700.17"
		Given an empty file named "filerighthere"
		And a file named "listlongfolder" with: 
		"""
		list -sl folderl
		exit
		"""
		Given a directory named "folderl/subfolder/subsubfolder" with mode "733" and date "201502140600.01"
		Given a directory named "folderl/subfolder" with mode "721" and date "201402140600.00"
		Given a directory named "folderl" with mode "712" and date "201302140600.00"
		When I run `../bin/wsh` interactively
		And I pipe in the file "listlongfolder"
		Then the stderr should not contain anything
		Then the output should match:
		"""
		-rwxrw----\s+4000\s+12/25/2016\s+07:00:16\s+file4000
		"""
		Then the output should match:
		"""
		---x-w--wx\s+5000\s+12/25/2017\s+07:00:17\s+file5000
		"""
		Then the output should match:
		"""
		drwx-w---x\s+4096\s+02/14/2014\s+06:00:00\s+subfolder
		"""
		Then the output should match:
		"""
		drwx-wx-wx\s+4096\s+02/14/2015\s+06:00:01\s+subsubfolder
		"""
		And the output should not contain "filerighthere"
		Then 20 points are awarded

