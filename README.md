# ReaderWriterProject <!-- omit in toc -->

> **CSE3033 – Operating Systems**
> · Marmara University Computer Engineering Department · 

A multithreaded C program that transforms a text file **in‑place** using four coordinated worker groups: **Read, Upper, Replace,** and **Write** threads. The assignment showcases pthread creation, semaphore‑based producer–consumer queues, and robust synchronisation to avoid race conditions and deadlocks, following the official project brief.

---

## Table of Contents

1. [Features](#features)
2. [Usage](#usage)
3. [Thread Workflow](#thread-workflow)


---

## Features

* **Parallel text processing** via user‑defined counts of each thread class (`Read`, `Upper`, `Replace`, `Write`).
* **Exactly‑once line handling:** every line is read, upper‑cased, space‑to‑underscore replaced, and written back exactly once.
* **Circular queues** protected by semaphores for stage hand‑off.
* **Fine‑grained line locks** (`pthread_mutex` per array slot) maximising concurrency while preventing simultaneous modification.
* **Verbose logging** that mirrors the example output in the brief.
* **Graceful shutdown:** main thread joins all workers; no orphan threads.

---

## Usage

```text
./project3.out -d <file> -n R U P W
```

* **`-d <file>`** — path to the text file to transform (modified **in‑place**).
* **`-n R U P W`** — numbers of **R**ead, **U**pper, **P**replace and **W**rite threads.

```bash
./project3.out -d deneme.txt -n 15 5 8 6
```

---

## Thread Workflow

| Stage       | Action                                                 | Delivers to |
| ----------- | ------------------------------------------------------ | ----------- |
| **Read**    | Acquire unique line index, read text into shared array | `upper_q`   |
| **Upper**   | Convert line to **uppercase**                          | `replace_q` |
| **Replace** | Replace spaces with **underscores**                    | `write_q`   |
| **Write**   | Seek to original offset and overwrite line in file     |  —          |

Queues are fixed‑size circular buffers; semaphores track items; a single `write_sem` ensures a line reaches **Write** only after both transformations.

---
