clear;
close all;

% Global variables
files(1,:) = '../../log/default.txt';
outputname = './images/default.png';
colors = [[0.3020 0.7451 0.9333];[0.9294 0.6941 0.1255];[0 0.6000 0.2000];[0.4941 0.1843 0.5569];[0.8510 0.3255 0.0980]];

fig = figure;
hold on;
for i=1:size(files,1)
    
    % Variables for this run
    file = fopen(char(files(i,:)));
    data = [];
    run = 1;

    % Skip header lines until we get to the first "Run"
    line = fgetl(file);
    while(size(line) < 3 | ~strncmpi(line,'Run',3))
        line = fgetl(file);
    end

    % Read in data
    while(line ~= -1)
        
        arg1 = [0];
        arg2 = [0];
        
        while(true)
            line = fgetl(file);

            if(strncmpi(line,'Run',3) | line == -1)
                break;
            end

            if(~isempty(line))
                lineData = cell2mat(textscan(line,'%f %f'));
                arg1 = [arg1,lineData(1)];
                arg2 = [arg2,lineData(2)];
            end
        end

        arg1 = [arg1,10000];
        arg2 = [arg2,arg2(end)];        
        
        % Graph data
        p = plot(arg1,arg2,'DisplayName',char(files(i,:)),'color',colors(i,:));
        hasbehavior(p,'legend',false);
    end
            
    fclose(file);
end

% Format graph
hold off;
%xlim([0,25]);
xlabel('Run');
ylabel('Fitness');
saveas(fig, outputname);
