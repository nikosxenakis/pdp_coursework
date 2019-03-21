#!/usr/bin/env python2
# data_analyzer.py
#B136013
#
import os
import matplotlib.pyplot as plt
import csv
from operator import itemgetter
from matplotlib.ticker import MultipleLocator

def create_plots(title, x_axis_title, y_axis_title, labels, x_values, y_values, alpha, legent_pos):

    fig, ax = plt.subplots()
    ax.set_xlabel(x_axis_title)
    ax.set_ylabel(y_axis_title)

    colors = ['#003f5c', '#444e86', '#955196', '#dd5182', '#ff6e54', '#ffa600']

    lines = []

    i = 0
    for y_list in y_values:
        x_list = x_values[0:len(y_list)]
        lines.append(plt.plot(x_list, y_list, linewidth=2, color=colors[i]))
        lines.append(plt.plot(x_list, y_list, label=labels[i], linewidth=2, color=colors[i]))
        i = i + 1

    # if title == "speedupBigInput":
    #     ml = MultipleLocator(36)
    #     ax.xaxis.set_minor_locator(ml)
    #     ax.xaxis.grid(which="minor", color='k', linestyle='-.', linewidth=0.7)

    plt.legend(loc=legent_pos)

    fig.savefig('./data/' + str(title) + '.eps', format='eps', dpi=1000)



def parse_population_influx(file, name):

    i = 0;

    title = ()
    timesteps = ()
    average = ()

    for line in file:
        data = line.strip().split('\t')

        if i == 0:
            titles = data
        else:
            timesteps = timesteps + (int(data[0]),)            
            average_tmp = -int(data[0])

            for value in data:
                average_tmp = average_tmp + int(value)
            average_tmp = average_tmp / 16

            average = average + (average_tmp,)

        i = i + 1


    # print(titles)
    # print(timesteps)
    # print(average)

    create_plots(
        name,
        "Month",
        name,
        [name],
        timesteps,
        [average],
        0.4,
        "upper left"
    )

path = './data/population_influx.tsv'
f = open(path, 'r')

parse_population_influx(f, "population_influx")

path = './data/infection_level.tsv'
f = open(path, 'r')

parse_population_influx(f, "infection_level")

