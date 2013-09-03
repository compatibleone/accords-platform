function [totals, profile, names] = read_profiles(directory)

  files = dir(strcat(directory, '*.csv'));
  for i = 1:length(files)
    names{i} = files(i).name;
    filename = files(i).name;    
    profile(i,:) = csvread(strcat(directory, filename))(2,:); #Discard the first line which is    
				#just the headings
  end
  totals = sum(profile);
  
 
  
