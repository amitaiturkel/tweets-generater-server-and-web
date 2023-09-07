# Tweet Server with C Integration and HTML Service

This project, known as **tweet_server**, combines two key components: a tweet generator, a server, and an HTML-based user interface. The tweet generator leverages C files within a Python environment to produce random tweets using Markov Chains. This README will primarily emphasize the tweet server, HTML file, and the integration of C code in Python.

## Table of Contents

1. [Introduction](#introduction)
2. [Usage](#usage)
3. [Code Structure](#code-structure)
4. [Compilation](#compilation)
5. [How It Works](#how-it-works)
6. [HTML User Interface](#html-user-interface)
7. [Customization](#customization)
8. [Contributing](#contributing)
9. [License](#license)
10. [Getting Started](#getting-started)

## Introduction

The **tweet_server** project combines three distinct functionalities:

1. **Tweet Generator**: This component uses C files to generate random tweets based on Markov Chains. The integration of C code within Python enables efficient and versatile text generation.

2. **FastAPI Server**: The server is built using FastAPI, a modern Python web framework. It allows users to request a specified number of random tweets from the Tweet Generator.

3. **HTML User Interface**: An HTML-based user interface is provided to interact with the server. Users can input the number of tweets they want and receive them through the web interface.

## Usage

To utilize the tweet server and generate tweets:
Clone this repository to your local machine by running the following command in your terminal:

```shell
`git clone https://github.com/amitaiturkel/calc-server-test](https://github.com/amitaiturkel/Tweet-Server-with-C-Integration-and-HTML-Service`


### Set Up the Virtual Environment

Activate a virtual environment to isolate the project's dependencies. If you're using `virtualenv`, you can create and activate the environment with the following commands:

`python3 -m venv .venv` 
or run `python -m venv .venv`
and then
`source .venv/bin/activate`


### Install Dependencies

Use `poetry` to install the project's dependencies:

`poetry install`


This will ensure that all required packages are installed.


1. Run the server using `uvicorn tweet_server:app --host=localhost --port=8000`.

2. Access the HTML-based user interface by opening `index.html` in a web browser. Enter the desired number of tweets and click "Generate Tweets."

## Code Structure

The codebase is organized into three main components:

1. **Tweet Generator**: The tweet generation functionality is encapsulated in C files and executed within Python. It utilizes a Markov Chain-based approach to create random tweets.

2. **FastAPI Server**: The server code, powered by FastAPI, handles incoming requests and communicates with the Tweet Generator to serve random tweets to clients.

3. **HTML User Interface**: The HTML file (`index.html`) provides a user-friendly interface for interacting with the server. JavaScript code within the HTML file sends requests to the server and displays the generated tweets.

## Compilation

The C code for tweet generation is compiled using a `Makefile`. The compilation process is seamlessly integrated into the Python environment, ensuring the efficient execution of C code within the project.

## How It Works

The project exemplifies the integration of C code in Python to achieve efficient tweet generation. The HTML user interface enhances user interaction by allowing them to input the number of tweets they want and receive them in real-time.

## HTML User Interface

The provided HTML user interface (`index.html`) offers a user-friendly way to interact with the tweet server. Users can input the number of tweets they desire and click "Generate Tweets" to receive them.

The interface is designed with the following features:

- Input Field: Users can enter the number of tweets they want to generate.
- "Generate Tweets" Button: Clicking this button sends a request to the server to generate the specified number of tweets.
- Display Area: The generated tweets are displayed in this area for users to view.

## Customization

The **tweet_server** project offers customization opportunities:

1. Adjust Tweet Generation Parameters: Experiment with different parameters to generate tweets with varying styles and content.

2. Extend Functionality: Extend the project by adding more C-based functionality or modifying the server to suit your specific needs.

3. Modify HTML Interface: Customize the HTML interface to match your project's branding and user experience requirements.

## Contributing

We welcome contributions to this project. If you have ideas for enhancements, bug fixes, or other improvements, please feel free to open an issue or submit a pull request.

## License

This project is licensed under the [INSERT LICENSE HERE] license. Please refer to the LICENSE file for more details.

## Getting Started

To get started with the project, follow these steps:


