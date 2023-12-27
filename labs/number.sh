#!/bin/bash

echo "This is a game to guess the randomly generated number."
echo "The random number is between 1 and 100."
echo "Generating..."

# Generate a random number between 1 and 100
random_number=$((RANDOM % 100 + 1))
# echo $random_number
echo "Successfully generated."

# Loop until the user guesses the correct number
while true; do
    read -p "Enter your guess: " user_number
    # echo $user_number

    # Check if valid
    # =~ for regex matching, [[]] for conditional expression, ^[0-9]+$ for matching numbers, "" for treating user_number as string
    if ! [[ "$user_number" =~ ^[0-9]+$ ]]; then
        echo "Invalid number. Try again."
        continue
    fi

    # Compare the user's guess with the random number
    if [ $user_number -lt $random_number ]; then
        echo "Try bigger number."
    elif [ $user_number -gt $random_number ]; then
        echo "Try smaller number."
    else
        echo "Congratulations! You guessed the correct number."
        break
    fi
done
