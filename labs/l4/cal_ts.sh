#!/usr/bin/bash
# Get the current year
current_year=$(date +'%Y')

# Loop through the past five years
for ((year = current_year; year > current_year - 5; year--)); do
    # Get the first and last timestamps for each year
    start_timestamp=$(date -u -d "$year-01-01" +'%s')
    end_timestamp=$(date -u -d "$year-12-31 23:59:59" +'%s')

    # Convert timestamps to human-readable date formats
    start_date=$(date -u -d "@$start_timestamp" +'%Y-%m-%d %H:%M:%S')
    end_date=$(date -u -d "@$end_timestamp" +'%Y-%m-%d %H:%M:%S')

    # Print the timestamp range for the current year
    echo "Year $year: Start: $start_timestamp, End: $end_timestamp"
done
