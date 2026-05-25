def balanced_diet():
    while True:
        # Read input
        cans = list(map(int, input().split()))
        n = cans[0]  # number of cans
        if n == 0:
            break

        calories = cans[1:]  # list of calorie values for the cans

        # Total sum of calories
        total_calories = sum(calories)
        target = total_calories // 2

        # DP array to track achievable sums up to target
        dp = [False] * (target + 1)
        dp[0] = True  # zero sum is always possible

        # Process each can's calories
        for cal in calories:
            for j in range(target, cal - 1, -1):
                if dp[j - cal]:
                    dp[j] = True

        # Find the largest sum <= target that is achievable
        best_sum = 0
        for i in range(target, -1, -1):
            if dp[i]:
                best_sum = i
                break

        # Calculate the calories for each meal
        meal1 = best_sum
        meal2 = total_calories - best_sum

        # Print result with meal1 (larger or equal to meal2)
        print(f"{max(meal1, meal2)} {min(meal1, meal2)}")


# Run the function
balanced_diet()
