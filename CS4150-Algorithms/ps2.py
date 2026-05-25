def find_c(n, t, segments):
    # Find the smallest speedometer reading to initialize the lower bound
    min_speedometer = min(s for d, s in segments)
    low = -min_speedometer  # To ensure positive true speed
    high = 1001000  # Large upper bound similar to the C code
    precision = 1e-9  # High precision to meet the error requirement

    while high - low > precision:
        mid = (low + high) / 2
        total_time = 0

        # Calculate total time for the journey with current guess for c (mid)
        for d, s in segments:
            if s + mid <= 0:
                total_time = float('inf') # Invalidate this mid if true speed is non-positive
                break
            total_time += d / (s + mid)

        # Compare the calculated time with the actual total time
        if total_time > t:
            low = mid  # We need to increase c
        else:
            high = mid  # We need to decrease c
    #print(f"low = {low}, high = {high}, mid = {mid}, total_time = {total_time}")
    return (low + high) / 2



# Input
n, t = map(int, input().split())
segments = [tuple(map(int, input().split())) for _ in range(n)]

# Find and print the constant c
c = find_c(n, t, segments)

print(f"{c:.9f}")

