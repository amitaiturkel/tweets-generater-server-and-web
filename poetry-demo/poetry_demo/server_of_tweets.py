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

def run_c_program(program_name, seed,path,num_of_words):
    parameters = [seed, num_of_words, path]
    run_command = [f"./{program_name}"] + [str(param) for param in parameters]
    subprocess.run(run_command, check=True)





def generate_tweets(num_tweets):
    current_directory = os.path.dirname(os.path.abspath(__file__))
    compile_c_program('tweets',current_directory)
    seed = 1
    current_directory = os.path.dirname(os.path.abspath(__file__))
    file_path = os.path.join(current_directory, "tweet_data_base.txt")

    # Define the output file path
    output_file_path = os.path.join(current_directory, "output.txt")

    run_c_program("tweets_generator", seed, file_path, num_tweets)

    # Read the content of the output file
    with open(output_file_path, "r") as output_file:
        tweets = output_file.read()

    return tweets

    

@app.post("/make_tweets", response_class=PlainTextResponse)
async def generate_tweets_endpoint(data: TweetRequest):

    try:
        num_tweets = data.numTweets
        print(num_tweets)
        # Generate tweets based on the received data
        tweets = generate_tweets(num_tweets)

        return tweets

    except Exception as e:
        raise HTTPException(status_code=400, detail=str(e))


if __name__ == "__main__":
    import uvicorn

uvicorn.run(app, host="localhost", port=8000)
