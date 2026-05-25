def main():
    while True:
        # Read n (heads) and m (knights)
        n, m = map(int, input().split())
        if n == 0 and m == 0:
            break

        # Read head diameters
        heads = [int(input()) for _ in range(n)]

        # Read knight heights
        knights = [int(input()) for _ in range(m)]

        # Sort the heads and knights
        heads.sort()
        knights.sort()

        # Try to assign knights to heads
        gold_coins = 0
        head_idx = 0
        knight_idx = 0

        while head_idx < len(heads) and knight_idx < len(knights):
            if knights[knight_idx] >= heads[head_idx]:
                # Knight can chop off the head
                gold_coins += knights[knight_idx]
                head_idx += 1  # Move to the next head
            knight_idx += 1  # Move to the next knight

        if head_idx == len(heads):
            # All heads are chopped off
            print(gold_coins)
        else:
            # Not enough knights to chop off all heads
            print("Loowater is doomed!")


if __name__ == "__main__":
    main()
