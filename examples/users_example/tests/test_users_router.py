from pytest import mark
from requests import Response, get


@mark.parametrize(
    "user_id_in,user_name_in",
    [(10, "Alex"), (100, "Name"), (10000000, "Text")],
)
def test_get_user_data(user_id_in: int, user_name_in: str) -> None:
    URI: str = f"http://0.0.0.0:8080/users/{user_id_in}/{user_name_in}/"
    response: Response = get(URI)
    assert response.status_code == 200
    json_response: dict[str, str] = response.json()
    assert json_response.get("id") == str(user_id_in)
    assert json_response.get("name") == user_name_in


@mark.parametrize(
    "name_in,message_in",
    [
        ("Alex", "mass"),
        ("Text", "description"),
        ("Test", "very long long long long long long long long long long description"),
    ],
)
def test_greet_user(name_in: str, message_in: str) -> None:
    URI: str = f"http://0.0.0.0:8080/users/{name_in}/"
    response: Response = get(URI, json={"message": message_in})
    assert response.status_code == 200
    json_response: dict[str, str] = response.json()
    assert json_response.get("Greetings") == f"Hello, {name_in}"
    assert json_response.get("Message") == message_in


@mark.parametrize(
    "name_in,field",
    [
        ("Alex", "mass"),
        ("Text", "description"),
        ("Test", "very long long long long long long long long long long description"),
    ],
)
def test_greet_user_error_422(name_in: str, field: str) -> None:
    URI: str = f"http://0.0.0.0:8080/users/{name_in}/"
    response: Response = get(URI, json={field: field})
    assert response.status_code == 422


@mark.parametrize(
    "uri",
    [("Alex"), ("asfsdfasdf"), ("asjdfjasdfasdf"), ("asdjfkjs d sd fsf"), (" "), ("")],
)
def test_error_404(uri: str) -> None:
    URI: str = f"http://0.0.0.0:8080/{uri}/"
    response: Response = get(URI)
    assert response.status_code == 404
