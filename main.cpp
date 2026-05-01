#include <iostream>
#include <string>
using namespace std;

#define MAX 1000

// Task Class
class Task {
public:
    string name;
    int hours;
    int credits;

    Task() {}
    Task(string n, int h, int c) {
        name = n;
        hours = h;
        credits = c;
    }
};

// Planner Class
class Planner {
private:
    Task tasks[MAX];
    int n;
    int adj[MAX][MAX];
    int indegree[MAX];

public:
    Planner() {
        n = 0;
        for (int i = 0; i < MAX; i++) {
            indegree[i] = 0;
            for (int j = 0; j < MAX; j++) {
                adj[i][j] = 0;
            }
        }
    }

    void addTask(Task t);
    void displayTasks();
    void greedy();
    void knapsack(int W);
    void addPrerequisite(int u, int v);
    void topologicalSort();
};

// Add Task
void Planner::addTask(Task t) {
    if (n < MAX) {
        tasks[n++] = t;
    }
}

// Display Tasks
void Planner::displayTasks() {
    cout << "Courses:\n";
    for (int i = 0; i < n; i++) {
        cout << tasks[i].name
             << " Hours: " << tasks[i].hours
             << " Credits: " << tasks[i].credits << endl;
    }
}

// Greedy Scheduling
void Planner::greedy() {
    Task temp[MAX];

    for (int i = 0; i < n; i++)
        temp[i] = tasks[i];

    // Sort by credits (descending)
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (temp[j].credits < temp[j + 1].credits) {
                swap(temp[j], temp[j + 1]);
            }
        }
    }

    int maxhours = 0;
    for (int i = 0; i < n; i++)
        maxhours = max(maxhours, temp[i].hours);

    int slot[MAX];
    for (int i = 0; i < maxhours; i++)
        slot[i] = -1;

    for (int i = 0; i < n; i++) {
        for (int j = temp[i].hours - 1; j >= 0; j--) {
            if (slot[j] == -1) {
                slot[j] = i;
                break;
            }
        }
    }

    cout << "Scheduled Courses:\n";
    for (int i = 0; i < maxhours; i++) {
        if (slot[i] != -1)
            cout << temp[slot[i]].name << " ";
    }
    cout << endl;
}

// Knapsack (DP)
void Planner::knapsack(int m) {
    int p[MAX], w[MAX];
    int dp[MAX][MAX] = {0};
    int x[MAX] = {0};

    for (int i = 1; i <= n; i++) {
        p[i] = tasks[i - 1].credits;
        w[i] = tasks[i - 1].hours;
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            if (w[i] <= j) {
                dp[i][j] = max(p[i] + dp[i - 1][j - w[i]], dp[i - 1][j]);
            } else {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }

    cout << "\nMaximum Credits: " << dp[n][m] << endl;

    int i = n, j = m;
    while (i > 0) {
        if (dp[i][j] != dp[i - 1][j]) {
            x[i] = 1;
            j -= w[i];
        }
        i--;
    }

    cout << "Selected Courses:\n";
    for (int k = 1; k <= n; k++) {
        if (x[k])
            cout << tasks[k - 1].name << endl;
    }
}

// Add Prerequisite
void Planner::addPrerequisite(int u, int v) {
    adj[u][v] = 1;
    indegree[v]++;
}

// Topological Sort
void Planner::topologicalSort() {
    int queue[MAX];
    int front = 0, rear = 0;
    int indeg[MAX];

    for (int i = 0; i < n; i++)
        indeg[i] = indegree[i];

    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0)
            queue[rear++] = i;
    }

    cout << "Course Order:\n";

    while (front < rear) {
        int u = queue[front++];
        cout << tasks[u].name << " ";

        for (int v = 0; v < n; v++) {
            if (adj[u][v]) {
                indeg[v]--;
                if (indeg[v] == 0)
                    queue[rear++] = v;
            }
        }
    }
    cout << endl;
}

// Main Function
int main() {
    Planner planner;
    int n;

    cout << "Enter number of courses: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        string name;
        int credits, hours;

        cout << "Enter details for Course " << i + 1 << endl;
        cout << "Name: ";
        cin >> name;
        cout << "Credits: ";
        cin >> credits;
        cout << "Hours Required: ";
        cin >> hours;

        planner.addTask(Task(name, hours, credits));
    }

    while (true) {
        int choice;

        cout << "\n1. Display Courses\n";
        cout << "2. Greedy Scheduling\n";
        cout << "3. Knapsack\n";
        cout << "4. Add Prerequisites\n";
        cout << "5. Topological Sort\n";
        cout << "6. Exit\n";

        cin >> choice;

        switch (choice) {
        case 1:
            planner.displayTasks();
            break;

        case 2:
            planner.greedy();
            break;

        case 3: {
            int capacity;
            cout << "Enter total available hours: ";
            cin >> capacity;
            planner.knapsack(capacity);
            break;
        }

        case 4: {
            int edges;
            cout << "Enter number of prerequisites: ";
            cin >> edges;
            cout << "Enter (u v):\n";

            for (int i = 0; i < edges; i++) {
                int u, v;
                cin >> u >> v;
                planner.addPrerequisite(u, v);
            }
            break;
        }

        case 5:
            planner.topologicalSort();
            break;

        case 6:
            return 0;
        }
    }
}
