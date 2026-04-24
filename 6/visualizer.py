import json
import matplotlib.pyplot as plt
from matplotlib.widgets import Button, RadioButtons, TextBox
from matplotlib.patches import Rectangle
import networkx as nx
import sys
import subprocess
import os

def get_tree_pos(nodes_list):
    if not nodes_list: return {}
    node_by_id = {n['id']: n for n in nodes_list}
    child_ids = set()
    for n in nodes_list:
        if n['left'] is not None: child_ids.add(n['left'])
        if n['right'] is not None: child_ids.add(n['right'])
    
    root_id = None
    for n in nodes_list:
        if n['id'] not in child_ids:
            root_id = n['id']
            break
            
    if root_id is None: return {}

    pos = {}
    def assign_pos(node_id, x, y, x_offset):
        if node_id is None: return
        pos[node_id] = (x, y)
        n = node_by_id[node_id]
        if n['left'] is not None:
            assign_pos(n['left'], x - x_offset, y - 1, x_offset / 1.8)
        if n['right'] is not None:
            assign_pos(n['right'], x + x_offset, y - 1, x_offset / 1.8)

    assign_pos(root_id, 0, 0, 1.0)
    return pos

current_step = 0
current_structure = "ArrayList"
steps = []
user_points = []

def load_data():
    global steps
    try:
        with open('steps.json', 'r', encoding='utf-8') as f:
            data = json.load(f)
        steps = data.get('steps', [])
    except Exception:
        steps = []

def main():
    global current_step, current_structure, user_points

    load_data()

    fig, ax = plt.subplots(figsize=(14, 8))
    plt.subplots_adjust(left=0.25, right=0.75, bottom=0.2)
    fig.canvas.manager.set_window_title("Lab 6 Visualizer - Menu")

    # Animation controls
    ax_prev = plt.axes([0.35, 0.05, 0.1, 0.075])
    ax_next = plt.axes([0.55, 0.05, 0.1, 0.075])
    btn_prev = Button(ax_prev, 'Previous')
    btn_next = Button(ax_next, 'Next')

    # Structure selector
    ax_radio = plt.axes([0.02, 0.4, 0.20, 0.25])
    radio = RadioButtons(ax_radio, ('ArrayList', 'LinkedList', 'BST', 'AVLTree', 'StdSetWrapper'))

    # GUI Mode Buttons
    ax_demo = plt.axes([0.8, 0.8, 0.15, 0.075])
    btn_demo = Button(ax_demo, 'Run Demo')

    ax_bench = plt.axes([0.8, 0.7, 0.15, 0.075])
    btn_bench = Button(ax_bench, 'Benchmark')

    ax_box = plt.axes([0.8, 0.5, 0.15, 0.075])
    text_box = TextBox(ax_box, 'X Y Z: ')

    ax_add = plt.axes([0.8, 0.4, 0.15, 0.075])
    btn_add = Button(ax_add, 'Add Point')

    ax_clear = plt.axes([0.8, 0.3, 0.15, 0.075])
    btn_clear = Button(ax_clear, 'Clear Points')

    def run_cpp_and_reload(args):
        try:
            subprocess.run(["lab6.exe"] + args, check=True)
        except FileNotFoundError:
            print("Помилка: lab6.exe не знайдено. Скомпілюйте код.")
            return
        except Exception as e:
            print(f"Помилка виконання: {e}")
            return
        
        load_data()
        global current_step
        current_step = max(0, len(steps) - 1)
        draw()

    def on_demo(event):
        global user_points
        user_points = []
        print("[*] Running Demo mode...")
        run_cpp_and_reload(["--demo"])

    def on_bench(event):
        try:
            ax.clear()
            ax.set_title("Запуск бенчмарку... Зачекайте.")
            ax.axis('off')
            fig.canvas.draw_idle()
            plt.pause(0.1)
            
            result = subprocess.run(["lab6.exe", "--benchmark"], capture_output=True, text=True, encoding='utf-8', errors='replace')
            output = result.stdout.strip() if result and result.stdout else ""
            if not output:
                output = "Немає результатів або сталася помилка."
            
            ax.clear()
            ax.set_title("Результати Бенчмарку (Час вставки 10 000 елементів)")
            ax.text(0.5, 0.5, output, ha='center', va='center', fontsize=12, family='monospace',
                    bbox=dict(boxstyle='round,pad=1', facecolor='lightyellow', edgecolor='gray'))
            ax.axis('off')
            fig.canvas.draw_idle()
            
        except Exception as e:
            print("Помилка виконання Benchmark:", e)

    def on_add(event):
        text = text_box.text.strip()
        if not text: return
        parts = text.split()
        if len(parts) == 3:
            try:
                x, y, z = map(float, parts)
                user_points.append(f"{x} {y} {z}")
                with open("input.txt", "w", encoding='utf-8') as f:
                    f.write("\n".join(user_points))
                
                text_box.set_val("")
                run_cpp_and_reload(["--file", "input.txt"])
            except ValueError:
                print("Введіть 3 числа через пробіл!")
        else:
            print("Потрібно 3 координати!")

    def on_clear(event):
        global user_points
        user_points = []
        with open("input.txt", "w", encoding='utf-8') as f:
            f.write("")
        run_cpp_and_reload(["--file", "input.txt"])

    def draw():
        ax.clear()
        if not steps: 
            ax.set_title("Немає даних. Натисніть 'Run Demo' або додайте точки.")
            ax.axis('off')
            fig.canvas.draw_idle()
            return
        
        step_data = steps[current_step]
        ax.set_title(f"Крок {current_step + 1}/{len(steps)}: Додано {step_data['inserted_point']}\nСтруктура: {current_structure}")
        
        struct_data = step_data['structures'].get(current_structure)
        if not struct_data: return
        
        if struct_data['type'] == 'array':
            nodes = struct_data['nodes']
            if not nodes: 
                ax.axis('off')
                fig.canvas.draw_idle()
                return
            
            box_width = 1.4
            box_height = 0.5
            max_boxes_per_row = 5
            for i, val in enumerate(nodes):
                row = i // max_boxes_per_row
                col = i % max_boxes_per_row
                x = col * (box_width + 0.1)
                y = -row * (box_height + 0.2)
                rect = Rectangle((x, y), box_width, box_height, facecolor='lightblue', edgecolor='black')
                ax.add_patch(rect)
                ax.text(x + box_width/2, y + box_height/2, val, ha='center', va='center', fontsize=9)
                
            ax.set_xlim(-0.5, max_boxes_per_row * (box_width + 0.1))
            num_rows = (len(nodes) - 1) // max_boxes_per_row + 1
            ax.set_ylim(-num_rows * (box_height + 0.2) - 0.5, 1)
            ax.axis('off')
            
        elif struct_data['type'] == 'tree':
            nodes = struct_data['nodes']
            if not nodes: 
                ax.axis('off')
                fig.canvas.draw_idle()
                return
            
            G = nx.DiGraph()
            labels = {}
            for n in nodes:
                G.add_node(n['id'])
                labels[n['id']] = n['val']
                if n['left'] is not None:
                    G.add_edge(n['id'], n['left'])
                if n['right'] is not None:
                    G.add_edge(n['id'], n['right'])
                    
            pos = get_tree_pos(nodes)
            if pos:
                nx.draw(G, pos, ax=ax, with_labels=True, labels=labels, 
                        node_size=2500, node_color='lightgreen', font_size=9,
                        arrows=True, arrowstyle='-|>', arrowsize=15)
            else:
                ax.text(0.5, 0.5, "Помилка відмальовки дерева", ha='center', va='center')
            ax.axis('off')

        fig.canvas.draw_idle()

    def on_prev(event):
        global current_step
        if current_step > 0:
            current_step -= 1
            draw()

    def on_next(event):
        global current_step
        if current_step < len(steps) - 1:
            current_step += 1
            draw()
            
    def on_radio_click(label):
        global current_structure
        current_structure = label
        draw()

    btn_prev.on_clicked(on_prev)
    btn_next.on_clicked(on_next)
    radio.on_clicked(on_radio_click)
    
    btn_demo.on_clicked(on_demo)
    btn_bench.on_clicked(on_bench)
    btn_add.on_clicked(on_add)
    btn_clear.on_clicked(on_clear)

    draw()
    plt.show()

if __name__ == "__main__":
    main()
