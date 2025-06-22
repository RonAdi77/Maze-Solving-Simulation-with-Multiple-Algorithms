import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


def plot_shortest_path(df_gen):
    """
        Plots the median shortest path length for each maze generation algorithm as maze size increases.
        Parameters:
            df_gen (pd.DataFrame): DataFrame containing 'Algo', 'Size', and 'Shortest Path' columns.

        Purpose:
            Helps visualize how different generation algorithms affect shortest path lengths as maze size changes.
        """
    avg_per_algo_and_size = df_gen.groupby(['Algo', 'Size'])['Shortest Path'].median().reset_index()

    plt.figure(figsize=(12, 7))
    for algo in avg_per_algo_and_size['Algo'].unique():
        data = avg_per_algo_and_size[avg_per_algo_and_size['Algo'] == algo]
        plt.plot(data['Size'], data['Shortest Path'], marker='o', label=algo)

    plt.title('Average Shortest Path Length by Maze Generator')
    plt.xlabel('Maze Size')
    plt.ylabel('Average Shortest Path Length')
    plt.legend(title='Generation Algorithm')
    plt.grid(True)
    plt.tight_layout()
    plt.show()

###################################

def plot_steps_vs_size(df_sol):
    """
    Plots average number of steps taken by each solution algorithm across different maze algorithms and sizes.
    Also overlays the average shortest path for reference.
    Parameters:
        df_sol (pd.DataFrame): DataFrame containing 'Solution Algo', 'Maze Algo', 'Size', 'Steps', and 'Maze Shortest Path'.

    Purpose:
        Compare actual solution steps vs optimal path lengths, highlighting the efficiency of various solution algorithms.
    """
    grouped = df_sol.groupby(['Solution Algo', 'Maze Algo', 'Size'])['Steps'].mean().reset_index()
    maze_algos = grouped['Maze Algo'].unique()

    reference_solution_algo = df_sol['Solution Algo'].unique()[0]

    shortest_path_grouped = df_sol[df_sol['Solution Algo'] == reference_solution_algo] \
        .groupby(['Maze Algo', 'Size'])['Maze Shortest Path'].mean().reset_index()

    fig, axes = plt.subplots(2, 2, figsize=(16, 12))
    axes = axes.flatten()

    for i, maze_algo in enumerate(maze_algos):
        ax = axes[i]
        maze_data = grouped[grouped['Maze Algo'] == maze_algo]

        for solution_algo in maze_data['Solution Algo'].unique():
            solution_data = maze_data[maze_data['Solution Algo'] == solution_algo]
            ax.plot(solution_data['Size'], solution_data['Steps'], marker='o', label=f'{solution_algo} - Steps')

        shortest_data = shortest_path_grouped[shortest_path_grouped['Maze Algo'] == maze_algo]
        ax.plot(shortest_data['Size'], shortest_data['Maze Shortest Path'],
                 marker='o', color='black', label='Shortest Path')

        ax.set_title(f'Maze Algorithm: {maze_algo}')
        ax.set_xlabel('Maze Size')
        ax.set_ylabel('Average Steps')
        ax.grid(True)
        ax.legend(title='Solution Algorithm')

    for j in range(len(maze_algos), len(axes)):
        fig.delaxes(axes[j])

    plt.tight_layout()
    plt.show()

###################################

def plot_steps_vs_size_linierfit(df_sol):
    """
    Plots log-log graphs of average steps vs maze size for each solution algorithm and fits a power law line.
    Also fits a line for the shortest path reference.
    Parameters:
        df_sol (pd.DataFrame): DataFrame containing 'Solution Algo', 'Maze Algo', 'Size', 'Steps', and 'Maze Shortest Path'.
    Purpose:
        To analyze the growth rate of each solution algorithm in comparison to the shortest path using linear regression in log-log space.
    """
    grouped = df_sol.groupby(['Solution Algo', 'Maze Algo', 'Size'])['Steps'].mean().reset_index()
    maze_algos = grouped['Maze Algo'].unique()
    reference_solution_algo = df_sol['Solution Algo'].unique()[0]

    shortest_path_grouped = df_sol[df_sol['Solution Algo'] == reference_solution_algo] \
        .groupby(['Maze Algo', 'Size'])['Maze Shortest Path'].mean().reset_index()

    fig, axes = plt.subplots(2, 2, figsize=(16, 12))
    axes = axes.flatten()

    color_map = plt.get_cmap('tab10')

    for i, maze_algo in enumerate(maze_algos):
        ax = axes[i]
        maze_data = grouped[grouped['Maze Algo'] == maze_algo]
        solution_algos = maze_data['Solution Algo'].unique()

        for j, solution_algo in enumerate(solution_algos):
            solution_data = maze_data[maze_data['Solution Algo'] == solution_algo]
            x = solution_data['Size'].values
            y = solution_data['Steps'].values

            color = color_map(j % 10)  # Up to 10 distinct colors

            ax.loglog(x, y, marker='o', linestyle='None', color=color, label=f'{solution_algo} data')
            log_x = np.log10(x)
            log_y = np.log10(y)
            coeffs = np.polyfit(log_x, log_y, 1)
            fit_y = 10 ** coeffs[1] * x ** coeffs[0]


            ax.loglog(x, fit_y, color=color, linewidth=2, alpha=0.8, label=f'{solution_algo} fit: slope={coeffs[0]:.2f}')


        shortest_data = shortest_path_grouped[shortest_path_grouped['Maze Algo'] == maze_algo]
        x_short = shortest_data['Size'].values
        y_short = shortest_data['Maze Shortest Path'].values

        ax.loglog(x_short, y_short, marker='o', linestyle='None', color='black', label='Shortest Path')


        log_x_short = np.log10(x_short)
        log_y_short = np.log10(y_short)
        coeffs_short = np.polyfit(log_x_short, log_y_short, 1)
        fit_y_short = 10 ** coeffs_short[1] * x_short ** coeffs_short[0]
        ax.loglog(x_short, fit_y_short, linewidth=2, color='gray',
                  label=f'Shortest Fit: slope={coeffs_short[0]:.2f}')

        ax.set_title(f'Maze Algorithm: {maze_algo}')
        ax.set_xlabel('Maze Size')
        ax.set_ylabel('Average Steps')
        ax.grid(True, which='both', alpha=0.6)
        ax.legend(title='Solution Algorithm', fontsize=9)


    for j in range(len(maze_algos), len(axes)):
        fig.delaxes(axes[j])

    plt.tight_layout()
    plt.show()

###################################

def plot_maze_complexity(df_gen):
    """
    Visualizes average maze complexity metrics: number of intersections (Ni), dead ends (Nde), and shortest path length.
    Parameters:
        df_gen (pd.DataFrame): DataFrame containing 'Algo', 'Ni', 'Nde', and 'Shortest Path'.
    Purpose:
        Helps compare how structurally complex each maze generation algorithm is.
    """
    comp = df_gen.groupby('Algo').agg({
        'Ni': 'mean',
        'Nde': 'mean',
        'Shortest Path': 'mean'
    }).reset_index()

    plt.figure(figsize=(10, 7))
    comp.set_index('Algo').plot(kind='bar', colormap='viridis', ax=plt.gca())

    plt.title('Maze Complexity by Generation Algorithm')
    plt.xlabel('Generation Algorithm')
    plt.ylabel('Average Values')
    plt.xticks(rotation=45)
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    plt.legend(['Intersections (Ni)', 'Dead Ends (Nde)', 'Shortest Path'])
    plt.tight_layout()
    plt.show()

###################################

def plot_steps_vs_multiple(df_multiple):
    """
    Plots the impact of increasing wall-to-path conversions (multiple k) on the number of steps required by each solution algorithm.
    Parameters:
        df_multiple (pd.DataFrame): DataFrame with 'Solution Algo', 'Maze Algo', 'Multiple', 'Steps', and 'Size'.
    Purpose:
        Evaluates how added connectivity affects solution efficiency across different solving algorithms.
    """
    maze_size = df_multiple['Size'].iloc[0]
    title_text = f"Maze Size: {maze_size}x{maze_size}"

    grouped = df_multiple.groupby(['Solution Algo', 'Maze Algo', 'Multiple'])['Steps'].mean().reset_index()
    maze_algos = grouped['Maze Algo'].unique()

    n_rows = 2
    n_cols = 2
    fig, axes = plt.subplots(n_rows, n_cols, figsize=(12, 8))
    axes = axes.flatten()

    for i, maze_algo in enumerate(maze_algos):
        ax = axes[i]
        solution_data = grouped[grouped['Maze Algo'] == maze_algo]

        for solution_algo in solution_data['Solution Algo'].unique():
            maze_data = solution_data[solution_data['Solution Algo'] == solution_algo]
            ax.plot(maze_data['Multiple'], maze_data['Steps'], marker='o', label=solution_algo)

        ax.set_title(f'Maze Algorithm: {maze_algo}')
        ax.set_xlabel('Multiple')
        ax.set_ylabel('Average Steps')
        ax.grid(True)
        ax.legend(title='Solution Algorithm', fontsize=9)


    for j in range(len(maze_algos), len(axes)):
        fig.delaxes(axes[j])

    plt.tight_layout(rect=[0, 0, 1, 0.95])
    fig.suptitle(title_text, fontsize=20, fontweight='bold')
    plt.subplots_adjust(top=0.88)
    plt.show()

###################################

def plot_ni_all_algos(df):
    """
    Plots number of intersections (Ni) as a function of maze size (log-log) with best-fit lines per generation algorithm.
    Parameters:
        df (pd.DataFrame): DataFrame with columns 'Algo', 'Size', and 'Ni'.
    Purpose:
        Understand how the number of intersections scales with maze size across generation algorithms.
    """
    grouped = df.groupby(['Algo', 'Size'])['Ni'].mean().reset_index()
    algo_list = grouped['Algo'].unique()

    plt.figure(figsize=(12, 8))
    for algo in algo_list:
        algo_data = grouped[grouped['Algo'] == algo]
        x = algo_data['Size'].values
        y = algo_data['Ni'].values

        plt.loglog(x, y, marker='o', linestyle='None', alpha=0.6, label=f'{algo} data')
        log_x = np.log10(x)
        log_y = np.log10(y)
        coeffs = np.polyfit(log_x, log_y, 1)
        fit_y = 10**(coeffs[1]) * x**(coeffs[0])
        plt.loglog(x, fit_y, linestyle='-', linewidth=2, label=f'{algo} fit: slope={coeffs[0]:.2f}')
        plt.plot(x, fit_y, linestyle='-', linewidth=2, label=f'{algo} fit: slope={coeffs[0]:.2f}')

    plt.title('Ni vs Maze Size (All Algorithms) with Best Fit Lines')
    plt.xlabel('Maze Size')
    plt.ylabel('Ni (Nodes Initiated)')
    plt.grid(True, which='both', linestyle='--', alpha=0.7)
    plt.legend(title='Algorithm')
    plt.tight_layout()
    plt.show()


###################################

def plot_nde_all_algos(df):
    """
    Plots number of dead ends (Nde) as a function of maze size (log-log) with best-fit lines per generation algorithm.
    Parameters:
        df (pd.DataFrame): DataFrame with columns 'Algo', 'Size', and 'Nde'.
    Purpose:
        Show how dead ends scale with maze size, indicating structural sparsity or complexity of generated mazes.
    """
    grouped = df.groupby(['Algo', 'Size'])['Nde'].mean().reset_index()
    algo_list = grouped['Algo'].unique()

    plt.figure(figsize=(12, 8))
    for algo in algo_list:
        algo_data = grouped[grouped['Algo'] == algo]
        x = algo_data['Size'].values
        y = algo_data['Nde'].values

        plt.loglog(x, y, marker='o', linestyle='None', alpha=0.6, label=f'{algo} data')

        log_x = np.log10(x)
        log_y = np.log10(y)
        coeffs = np.polyfit(log_x, log_y, 1)
        fit_y = 10**(coeffs[1]) * x**(coeffs[0])

        plt.loglog(x, fit_y, linestyle='-', linewidth=2, label=f'{algo} fit: slope={coeffs[0]:.2f}')

    plt.title('Nde vs Maze Size (All Algorithms) with Best Fit Lines')
    plt.xlabel('Maze Size')
    plt.ylabel('Nde (Dead Ends)')
    plt.grid(True, which='both', linestyle='--', alpha=0.7)
    plt.legend(title='Algorithm')
    plt.tight_layout()
    plt.show()


def main():
    df_gen = pd.read_csv("no_multiple_generators.csv")
    df_sol = pd.read_csv("no_multiple_solution.csv")
    df_multiple = pd.read_csv("multiple_solution.csv")

    plot_shortest_path(df_gen)
    plot_steps_vs_size(df_sol)
    plot_steps_vs_size_linierfit(df_sol)
    plot_maze_complexity(df_gen)
    plot_steps_vs_multiple(df_multiple)
    plot_ni_all_algos(df_gen)
    plot_nde_all_algos(df_gen)

if __name__ == "__main__":
    main()
