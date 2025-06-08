package com.example.curtainopener

import androidx.lifecycle.ViewModel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.update
import kotlinx.serialization.Serializable
import kotlinx.serialization.json.Json
import okhttp3.Call
import okhttp3.Callback
import okhttp3.OkHttpClient
import okhttp3.Request
import okhttp3.Response
import java.io.IOException
import java.time.LocalTime
import java.time.ZoneId
import java.time.Instant
import java.time.format.DateTimeFormatter

@Serializable
data class StatusJson(
    val alarmStatus: Boolean,
    val time: Long,
    val temperature: Double,
    val humidity: Double,
    val openingTimes: MutableList<String>
)

data class GetStatusUiState(
    val alarmStatus: Boolean = false,
    val time: LocalTime? = LocalTime.now(),
    val timeCorrect: Boolean = false,
    val temperature: Double = 21.3,
    val humidity: Double = 43.5,
    val openingTimes: List<String> = listOf<String>("")
)

class HttpClientViewModel : ViewModel() {
    private val client = OkHttpClient()
    private val _uiState = MutableStateFlow(GetStatusUiState())
    val uiState: StateFlow<GetStatusUiState> = _uiState.asStateFlow()

    fun getStatus() {
        val request = Request.Builder().url("http://192.168.1.142:8000/status").build()

        client.newCall(request).enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                e.printStackTrace()
            }

            override fun onResponse(call: Call, response: Response) {
                response.use {
                    if (!response.isSuccessful) throw IOException("Unexpected code $response")

                    _uiState.update { currentState ->
                        response.body?.let { it1 ->
                            val status = Json.decodeFromString<StatusJson>(it1.string())
                            val time = Instant.ofEpochSecond(status.time)
                                .atZone(ZoneId.systemDefault()).toLocalTime()
                            val dtf: DateTimeFormatter = DateTimeFormatter.ofPattern("HH:mm:ss")
                            // probably not ideal
                            val timeCorrect = time.toString() == LocalTime.now().format(dtf)
                            val first = status.openingTimes.removeAt(0)
                            status.openingTimes.add(first)
                            val openingTimes = status.openingTimes

                            currentState.copy(
                                alarmStatus = status.alarmStatus,
                                time = time,
                                timeCorrect = timeCorrect,
                                temperature = status.temperature,
                                humidity = status.humidity,
                                openingTimes = openingTimes
                            )
                        }!!
                    }
                }
            }
        })
    }
}