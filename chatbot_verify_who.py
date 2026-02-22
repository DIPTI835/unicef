import re
import time
import pandas as pd
from newspaper import build
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.metrics.pairwise import cosine_similarity
import sys
import json

WHO_SITES = [
    "https://www.who.int/news",
    "https://www.who.int/emergencies/disease-outbreak-news",
]

CACHE_CSV = "who_news_cache.csv"
MAX_ARTICLES = 100
REFRESH_EVERY_SECONDS = 60 * 60 * 12  # 12 hours

def clean_text(t: str) -> str:
    t = t.lower()
    t = re.sub(r"\s+", " ", t)
    t = re.sub(r"[^a-z0-9\s.,%-]", " ", t)
    return t.strip()

def scrape_who_news(max_articles=MAX_ARTICLES) -> pd.DataFrame:
    rows = []
    for site in WHO_SITES:
        paper = build(site, memoize_articles=False)
        count = 0
        for art in paper.articles:
            if count >= max_articles:
                break
            try:
                art.download()
                art.parse()
                text = clean_text(art.text or "")
                if len(text) < 400:  # skip very short pages
                    continue
                rows.append({
                    "url": art.url,
                    "title": clean_text(art.title or ""),
                    "publish_date": art.publish_date.isoformat() if art.publish_date else "",
                    "text": text,
                })
                count += 1
            except Exception:
                continue
    df = pd.DataFrame(rows).drop_duplicates(subset=["url"])
    return df

def load_or_refresh_cache() -> pd.DataFrame:
    try:
        df = pd.read_csv(CACHE_CSV)
        if df.empty:
            raise FileNotFoundError
        return df
    except Exception:
        df = scrape_who_news()
        df.to_csv(CACHE_CSV, index=False)
        return df

class WhoVerifier:
    def __init__(self, refresh=True):
        self.df = load_or_refresh_cache() if refresh else pd.read_csv(CACHE_CSV)
        self.vectorizer = TfidfVectorizer(
            ngram_range=(1, 2),
            stop_words="english",
            min_df=2,
            max_df=0.8
        )
        self.corpus = self.df["text"].tolist()
        self.X = self.vectorizer.fit_transform(self.corpus)

    def verify(self, claim: str, top_k=3, threshold=0.18):
        claim_clean = clean_text(claim)
        if not claim_clean:
            return {"status": "error", "message": "Empty claim."}

        q = self.vectorizer.transform([claim_clean])
        sims = cosine_similarity(q, self.X).flatten()
        idx_sorted = sims.argsort()[::-1][:top_k]
        results = []
        for i in idx_sorted:
            results.append({
                "score": float(sims[i]),
                "title": self.df.iloc[i]["title"],
                "url": self.df.iloc[i]["url"],
                "snippet": self.df.iloc[i]["text"][:400] + "..."
            })

        best = results[0] if results else None
        if best and best["score"] >= threshold:
            return {
                "status": "matched",
                "confidence": round(best["score"], 3),
                "evidence": results
            }
        else:
            return {
                "status": "no_match",
                "confidence": round(best["score"], 3) if best else 0.0,
                "message": "Could not verify this claim against recent WHO news."
            }

# php
if __name__ == "__main__":
    import sys, json, traceback
    print("DEBUG: Script started")  # Always prints

    try:
        if len(sys.argv) > 1:
            claim = " ".join(sys.argv[1:])
            print("DEBUG: Claim received:", claim)
            bot = WhoVerifier(refresh=True)
            result = bot.verify(claim)
            print("DEBUG: Verification complete")
            print(json.dumps(result, indent=2))
        else:
            print(json.dumps({"status": "error", "message": "No claim provided"}))
    except Exception as e:
        print("ERROR:", e)
        traceback.print_exc()