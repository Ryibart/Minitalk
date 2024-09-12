#!/bin/bash

# Path to your client executable
CLIENT_PATH="../client"

# Emoji to send
EMOJI="🧹"

# Prompt the user to enter the server PID
read -p "Enter the server PID: " SERVER_PID

# Check if the provided PID is a valid number
if ! [[ "$SERVER_PID" =~ ^[0-9]+$ ]]; then
    echo "Error: Invalid PID. Please enter a numeric PID."
    exit 1
fi

# Initialize counter
count=0

# Function to send number and emoji to the server
send_number_and_emoji() {
    MESSAGE="$count $EMOJI"
    $CLIENT_PATH "$SERVER_PID" "$MESSAGE"
    # Check if the client exited with an error
    if [ $? -ne 0 ]; then
        echo "Error: Client exited with an error. Exiting script."
        exit 1
    fi
}

# Main loop to continuously send number and emoji
while true; do
    send_number_and_emoji
    # Increment the counter
    count=$((count + 1))
    # Print the number and emoji sent
    echo "Sent: $count $EMOJI"
    # Sleep for 0.03 seconds between sends (adjust as needed)
    sleep 0.02
done