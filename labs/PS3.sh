#!/bin/bash

PS3="Choose one option: "
options=("Option A" "Option B" "Option C" "Quit")

select choice in "${options[@]}"; do
    case $choice in
        "Option A")
            echo "You chose Option A"
            ;;
        "Option B")
            echo "You chose Option B"
            ;;
        "Option C")
            echo "You chose Option C"
            ;;
        "Quit")
            echo "Exiting..."
            break
            ;;
        *)
            echo "Invalid option"
            ;;
    esac
done
