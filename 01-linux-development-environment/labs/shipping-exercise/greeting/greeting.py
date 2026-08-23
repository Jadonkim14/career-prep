import typer

def greet(name: str):
    print(f"Hello, {name}!")

def cli():
    typer.run(greet)
