from typing import Annotated, Dict

from requests import request
from pydantic import BaseModel
from fastapi import FastAPI, Header, HTTPException
from fastapi.openapi.utils import get_openapi


app = FastAPI()



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
    tweetLength: str

def generate_tweets(num_tweets, tweet_length):
    # This is a placeholder function, replace it with your logic
    tweets = []
    for i in range(num_tweets):
        tweet = f"Generated Tweet {i + 1} of length {tweet_length}"
        tweets.append(tweet)
    return tweets
    

users: Dict[str, str] = {}
@app.post("/make_tweets", response_model=dict)
async def generate_tweets_endpoint(data: TweetRequest):
    try:
        num_tweets = data.numTweets
        tweet_length = data.tweetLength

        # Generate tweets based on the received data
        tweets = generate_tweets(num_tweets, tweet_length)

        return {"tweets": tweets}

    except Exception as e:
        raise HTTPException(status_code=400, detail=str(e))


@app.get("/users")
async def read_item1():
    return {"result": users}


if __name__ == "__main__":
    import uvicorn

    uvicorn.run(app, host="0.0.0.0", port=8000)
