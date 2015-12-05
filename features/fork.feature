Feature: External (unknown) commands are executed with fork(), exec() and wait() 
	Scenario: External command not found error is printed from perror()
		Given a file named "executeboguscommand" with:
		"""
		IamAbogusCommand
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "executeboguscommand"
		Then the output should contain: 
		"""
		No such file or directory
		"""
		Then 5 points are awarded

	Scenario: External command is executed with fork(), exec() and wait() with all optional arguments
		Given a file named "executecommand" with:
		"""
		whoami
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "executecommand"
		Then the output should contain my username 
		Then 15 points are awarded


	Scenario: External command is executed with fork(), exec() and wait() with all optional arguments
		Given a file named "executecommandwithargs" with:
		"""
		/bin/echo "Ted says I should work hard to get an A"
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "executecommandwithargs"
		Then the output should contain: 
		"""
		"Ted says I should work hard to get an A"
		"""
		Then the output should not contain: 
		"""
		/bin/echo "
		"""
		Then 30 points are awarded	
