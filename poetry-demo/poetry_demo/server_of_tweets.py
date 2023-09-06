from typing import Annotated, Dict
import subprocess
from requests import request
from pydantic import BaseModel
from fastapi import FastAPI, Header, HTTPException
from fastapi.openapi.utils import get_openapi
import os
from fastapi.responses import PlainTextResponse
from fastapi.middleware.cors import CORSMiddleware




app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # Allow requests from any origin (use "*" for all)
    allow_methods=["*"],  # Allow all HTTP methods
    allow_headers=["*"],  # Allow all HTTP headers
)

def custom_openapi():
    if app.openapi_schema:
        return app.openapi_schema
    openapi_schema = get_openapi(
        title="Custom title",
        version="2.5.0",
        summary="This is a very custom OpenAPI schema",
        description="Here's a longer description of the custom **OpenAPI** schema",
        routes=app.routes,
    )
    openapi_schema["info"]["x-logo"] = {"url": "https://fastapi.tiangolo.com/img/logo-margin/logo-teal.png"}
    app.openapi_schema = openapi_schema
    return app.openapi_schema


app.openapi = custom_openapi

class TweetRequest(BaseModel):
    numTweets: int


def compile_c_program(target,path):
    compile_command = ["make",target]
    subprocess.run(compile_command, check=True,cwd=path)

##compile_c_program("tweets")


import subprocess

def run_c_program(program_path, seed, database_path, num_of_words,current_directory):
    # Convert parameters to strings
    seed_str = str(seed)
    num_of_words_str = str(num_of_words)
    # Create the command as a list of strings
    command = [program_path, seed_str, num_of_words_str, database_path]
    try:
        result = subprocess.run(command, check=True, stderr=subprocess.PIPE, text=True,cwd = current_directory)
        # Print stderr (error messages) if there are any
        if result.stderr:
            print(f"Error messages from {program_path}:\n{result.stderr}")
    except subprocess.CalledProcessError as e:
        print(f"Error: {e}")







def generate_tweets(num_tweets):
    current_directory = os.path.dirname(os.path.abspath(__file__))
    compile_c_program('tweets',current_directory)
    seed = 1
    current_directory = os.path.dirname(os.path.abspath(__file__))
    database_path = os.path.join(current_directory, "tweet_data_base.txt")
    program_path = os.path.join(current_directory, "tweets_generator")

    # Define the output file path
    output_file_path = os.path.join(current_directory, "output.txt")
    # Check if the output file exists
    if os.path.exists(output_file_path):
    # If the file exists, open it in write mode to truncate its content
        with open(output_file_path, "w") as output_file:
        # Truncate the file by writing an empty string
            output_file.write("")
    
    run_c_program(program_path, seed, database_path, num_tweets,current_directory)

    # Read the content of the output file
    with open(output_file_path, "r") as output_file:
        tweets = output_file.read()
        

    return tweets

    

@app.post("/make_tweets", response_class=PlainTextResponse)
async def generate_tweets_endpoint(data: TweetRequest):

    try:
        num_tweets = data.numTweets
        # Generate tweets based on the received data
        tweets = generate_tweets(num_tweets)

        return tweets

    except Exception as e:
        raise HTTPException(status_code=400, detail=str(e))


if __name__ == "__main__":
    import uvicorn

uvicorn.run(app, host="localhost", port=8000)
