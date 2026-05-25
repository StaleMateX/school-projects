from collections import deque, defaultdict


def will_country_leave(c, p, x, l, partnerships):
    # Initialize graph and tracking structures
    graph = defaultdict(list)
    initial_trade_count = {}
    left_trade_count = {}
    has_left = [False] * (c + 1)

    # Build the graph from the partnerships
    for a, b in partnerships:
        graph[a].append(b)
        graph[b].append(a)

    # Initialize the initial trade counts for each country
    for country in range(1, c + 1):
        initial_trade_count[country] = len(graph[country])
        left_trade_count[country] = 0

    # BFS queue for the countries leaving the union
    queue = deque([l])
    has_left[l] = True

    while queue:
        current_country = queue.popleft()

        for neighbor in graph[current_country]:
            if has_left[neighbor]:
                continue
            # Increase the count of partners who have left for the neighbor
            left_trade_count[neighbor] += 1

            # Check if the neighbor will leave
            if left_trade_count[neighbor] >= (initial_trade_count[neighbor] / 2):
                has_left[neighbor] = True
                queue.append(neighbor)

    # Output result for country X
    return "leave" if has_left[x] else "stay"


# Input reading and function calling
c, p, x, l = map(int, input().split())
partnerships = [tuple(map(int, input().split())) for _ in range(p)]
print(will_country_leave(c, p, x, l, partnerships))
