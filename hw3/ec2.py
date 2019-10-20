[N, P] = map(int, input().split(" "))              # N: number of colors/people; P: id of current person
possible_colors = input().split(" ")               # All possible colors
view = [c for c in input().split(" ") if c != ""]  # Hat colors this person sees (if there's only one player, we may see nothing!)

# ... code up your strategy here & print out your guess ...
d = {possible_colors[i]:i for i in range(N)}
# guess a sum
guessed_sum = P
# solve guessed_sum = my_hat + hat_sum
# for my_hat, the person who guesses the correct sum
# can solve for their own hat color.
print(possible_colors[(guessed_sum-sum(d[v] for v in view))%N])
