Given /^the output should contain PWD$/ do 
	#puts ENV['PWD']
	#puts ENV['HOME']
	#puts @dirs.join("/")
	#step "the output should match /" + Regexp.escape(ENV['PWD']) + "/"
	step "the output should contain \"" +  ENV['PWD'] + "/" + @dirs.join("/") + "\""
end

Given /^(.*) points are awarded/ do |points|
	#puts "#{points} points are now awarded!!!"
	$total_points += points.to_i
end

Given /^dot is replaced with PWD\/(.*)$/ do |outputfile|
	step "the output should contain \"copy #{outputfile} #{ENV['PWD']}\/#{File.join(@dirs)}\/#{outputfile}\""
end

Given /^a directory named "([^"]*)" with mode "([^"]*)" and date "([^"]*)"$/ do |dir_name, dir_mode, dir_date|
  create_dir(dir_name)
  filesystem_permissions(dir_mode, dir_name)
  `touch -t #{dir_date} tmp/aruba/#{dir_name}`
end

Given /^a (\d+) byte file named "([^"]*)" with mode "([^"]*)" and date "([^"]*)"$/ do |file_size, file_name, file_mode, file_date|
  write_fixed_size_file(file_name, file_size.to_i)
  filesystem_permissions(file_mode, file_name)
  `touch -t #{file_date} tmp/aruba/#{file_name}`
end

Given /^the output should contain my username$/ do
	me=`whoami`
	step "the output should contain \"#{me}\""
end

