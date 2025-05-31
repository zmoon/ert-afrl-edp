FROM python:3.12-slim

# Install uv
COPY --from=ghcr.io/astral-sh/uv:latest /uv /uvx /usr/local/bin/

# Set working directory
WORKDIR /app

# Copy the Python script
COPY rtsw.py .

# Create fetches directory for data storage
RUN mkdir -p fetches

# Set environment variables
ENV PYTHONUNBUFFERED=1
ENV UV_SYSTEM_PYTHON=1

# Create a non-root user for security
RUN useradd --create-home --shell /bin/bash app && \
    chown -R app:app /app
USER app

# Default command -- can be overridden in K8s
CMD ["uv", "run", "--script", "./rtsw.py"]
