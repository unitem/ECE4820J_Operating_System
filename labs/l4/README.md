# Lab4 Non-programming Exercises

## 1.2

1. Most common database systems: MySQL, Oracle Database, Microsoft SQL Server, PostgreSQL, MongoDB, SQLite, Redis, Cassandra, MariaDB, Amazon Aurora, IBM Db2, SQLite
2. Pros and Cons:
    1. MySQL: 
        Pros:
        - Open-source and widely used, with a large community.
        - Fast and reliable for simple to moderately complex applications.
        - Good support for structured data and SQL queries.
  
        Cons:
        - Limited support for complex transactions and analytical processing.
        - Less feature-rich compared to some enterprise-level systems.
        - Ownership and development changes over the years.

    2. PostgreSQL:

        Pros:
        - Open-source and highly extensible with a strong community.
        - Advanced features, including support for complex data types.
        - Excellent support for ACID compliance, complex queries, and transactions.

        Cons:
        - Slightly steeper learning curve compared to MySQL.
        - Can require more hardware resources for very high loads.
        - May not be as performant as NoSQL databases for certain use cases.

    3. MongoDB:

        Pros:
        - Flexible, NoSQL document database, great for unstructured or semi-structured data.
        - Scales horizontally for high read and write loads.
        - Good for real-time analytics and high-velocity data.

        Cons:
        - Lack of support for complex transactions.
        - Schemaless design can lead to data integrity challenges.
        - May not be suitable for applications requiring ACID compliance.
3.  Create an empty SQLite database.

```bash
sqlite3 l4.db
```

4. Use the SQLite shell to prepare two empty tables for each of your .csv file.
```SQL
CREATE TABLE db
(
    hash TEXT NOT NULL,
    name TEXT NOT NULL,
    comment TEXT NOT NULL  
);
CREATE TABLE time_stamp
(
    hash TEXT NOT NULL,
    name TEXT NOT NULL,
    dates TEXT,
    tstamp INT
);
```

5. Import each .csv file in its corresponding SQLite table.
```SQL
.separator "|"
.import db.psv db
.import timestamp.psv time_stamp
```
## 1.3

1. Who are the top five contributors to the Linux kernel since the beginning?
```SQL
SELECT name FROM (SELECT name, COUNT(name) AS count FROM db GROUP BY name ORDER BY count DESC LIMIT 5);
```
The above codes will return the top five contributors' name.
```text
Linus Torvalds
David S. Miller
Takashi Iwai
Mark Brown
Arnd Bergmann
```
2. Who are the top five contributors to the Linux kernel for each year over the past five years?

First, I use a script to print the timestamp ranges for the past five years. See `/l4/cal_ts.sh`.

The result of it is:

```text
Year 2023: Start: 1672531200, End: 1704067199
Year 2022: Start: 1640995200, End: 1672531199
Year 2021: Start: 1609459200, End: 1640995199
Year 2020: Start: 1577836800, End: 1609459199
Year 2019: Start: 1546300800, End: 1577836799
```

Then I can run 
```SQL
SELECT name, COUNT(name) AS count FROM time_stamp WHERE tstamp BETWEEN 1640995200 AND 1672531199 GROUP BY name ORDER BY count DESC LIMIT 5;
```

The result is
```text
# 2019
Linus Torvalds|2386
David S. Miller|1206
Chris Wilson|1173
YueHaibing|930
Christoph Hellwig|911
# 2020
Linus Torvalds|1886
David S. Miller|924
Christoph Hellwig|806
Mauro Carvalho Chehab|770
Chris Wilson|644
# 2021
# None
# 2022
# None
# 2023
# None
```
Probably because the data is not complete, so there is no result for 2021, 2022, 2023. However, except for "for the past five years", my approach can get the result for any year.

3. What is the most common “commit subject”?

If the comment is the commit subject, then the answer is
```SQL
SELECT comment FROM (SELECT comment, COUNT(name) AS count FROM db GROUP BY comment ORDER BY count DESC LIMIT 1);
```
The result is
```text
Merge git://git.kernel.org/pub/scm/linux/kernel/git/davem/net
```

4. On which day is the number of commits the highest?

```SQL
SELECT COUNT(name) AS count, date(dates) AS day FROM time_stamp GROUP BY day ORDER BY count DESC LIMIT 5;
```

```text
1031|2008-01-30
683|2006-12-07
649|2007-05-08
626|2013-07-03
613|2007-10-16
```

So the result is 2008-01-30.

5. Determine the average time between two commits for the five main contributors.
```SQL
SELECT name, COUNT(name) AS count, MAX(tstamp) AS latest_time, MIN(tstamp) AS earliest_time FROM time_stamp GROUP BY name ORDER BY count DESC LIMIT 5;
```
We can see the result is
```text
Linus Torvalds|30702|1601242690|1113690036
David S. Miller|13180|1600735253|1113690241
Takashi Iwai|7726|1600683992|1111682910
Mark Brown|7670|1598466419|1138838401
Arnd Bergmann|7520|1596184915|1116420059
```
So the average time is 
Linus Torvalds: (1601242690-1113690036)/(30702-1)=15,880s=4.41h
David S. Miller: (1600735253-1113690241)/(13180-1)=36,956s=10.27h
Takashi Iwai: (1600683992-1111682910)/(7726-1)=63,301s=17.58h
Mark Brown: (1598466419-1138838401)/(7670-1)=59,933s=16.65h
Arnd Bergmann: (1596184915-1116420059)/(7520-1)=6,373s=17.72h

## 2

1. Compile and debug using 
```bash
gcc -g -o myprogram myprogram.c
gdb ./myprogram
```

2. GDB stands for "GNU Debugger." Its functions includes 
    1. Set breakpoints
    2. Examine variables
    3. Step through code
    4. Inspect memory
    5. Handle multi-threading
    6. Post-mortem analysis
    7. Customize debugging sessions

3. GDB usage
    1. homepage: https://www.gnu.org/software/gdb/
    2. supported languages: Ada, Assembly, C, C++, D, Fortran, Go, Objective-C, OpenCL, Modula-2, Pascal, Rust.