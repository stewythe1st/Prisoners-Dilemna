clear;
close all;

% Global variables
file1 = '../../log/default.txt.csv';
file2 = '../../log/default.txt_best.csv';
outputname = './images/default.png';

% Read in data
data1 = csvread(file1,1,0);
data2 = csvread(file2,1,0);

% Narrow down to only data from the best run
[max,max_idx] = max(data1);
[min,min_idx] = min(data1);
best_run = data1(max_idx(4),:);
best_run = best_run(1);
idxs = find(data1(:,1)==best_run);
best_data = data1(idxs,:);

fig = figure('rend','painters','pos',[100 100 1000 400]); 

% Plot data
subplot(1,2,1);
hold on;
plot(best_data(:,2),best_data(:,3),'--','color','blue');
plot(best_data(:,2),best_data(:,4),'-','color','blue');
hold off;
xlabel('Run');
ylabel('Fitness');
title('Fitness vs Evals');

% Parse boxplot data
idxs = find(data2(:,1)==best_run);
box_data = data2(idxs,:);
subplot(1,2,2);
boxplot(box_data(:,3));
title('Distribution of Final Population');
ylabel('Fitness');

% Save image
fig.PaperPositionMode = 'auto';
print(outputname,'-dpng','-r0')
